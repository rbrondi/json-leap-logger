#include "leaplogger.h"


#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include <chrono>

using namespace Leap;

const std::string LeapLogger::fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string LeapLogger::boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string LeapLogger::stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

LeapLogger::LeapLogger(const QString& fn, const LoggerConfig& config): QObject(),
    mF(fn),
    mConfig(config),
    mCounter(0),
    mController() {

    if (!this->mF.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw std::runtime_error("impossibile aprire il file");
    }

    this->mOut = new QTextStream(&this->mF);
}

LeapLogger::~LeapLogger(){
    // Remove the listener when done
    this->mController.removeListener(*this);


    //close the json array
    (*this->mOut)<< "\n]";
    this->mOut->flush();
    if(this->mOut)
        delete this->mOut;
}

void LeapLogger::connect2Leap(){
    //open the json array
    (*this->mOut)<< "[\n";

    // registering the logger for events
    this->mController.addListener(*this);

    //  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    //    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
}

QJsonObject LeapLogger::serializeVector(const Leap::Vector v){
    return QJsonObject {
        {"x", v.x},
        {"y", v.z},
        {"z", v.z}
    };
}

void LeapLogger::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
}

void LeapLogger::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
//    controller.enableGesture(Gesture::TYPE_CIRCLE);
//    controller.enableGesture(Gesture::TYPE_KEY_TAP);
//    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
//    controller.enableGesture(Gesture::TYPE_SWIPE);
}

void LeapLogger::onDisconnect(const Controller& controller) {
    // Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
    emit this->disconnected();
}

void LeapLogger::onExit(const Controller& controller) {
    std::cout << "Exited" << std::endl;
    emit this->disconnected();
}

void LeapLogger::onFrame(const Controller& controller) {

    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();
    std::cout << "Frame id: " << frame.id();
    
    QJsonObject obj {
        {"frame_id", (int)frame.id()},
    };

    if(this->mConfig.isLoggingTimestamp()){
        //obj["timestamp"] = QString::number(QDateTime::currentDateTime().toTime_t());
        std::chrono::time_point<std::chrono::steady_clock> tp = std::chrono::steady_clock::now();
        double ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
        obj["timestamp"] = ms;
    }

    HandList hands = frame.hands();
    QJsonArray handList;
    for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
        // Get the first hand
        QJsonObject jHand;
        const Hand hand = *hl;
        jHand["id"] = hand.id();
        jHand["type"] = hand.isLeft() ? "l" : "r";
        jHand["palm"] = this->serializeVector(hand.palmPosition());
        jHand["palmWidth"] = hand.palmWidth();

        // Get the hand's normal vector, direction and basis
        const Vector normal = hand.palmNormal();
        const Vector direction = hand.direction();
		jHand["normal"] = this->serializeVector(hand.palmNormal());
		jHand["direction"] = this->serializeVector(hand.direction());
		jHand["xBasis"] = this->serializeVector(hand.basis().xBasis);
		jHand["yBasis"] = this->serializeVector(hand.basis().yBasis);
		jHand["zBasis"] = this->serializeVector(hand.basis().zBasis);

        // Calculate the hand's pitch, roll, and yaw angles
        jHand["pitch"] = direction.pitch() * RAD_TO_DEG;
        jHand["roll"] = normal.roll() * RAD_TO_DEG;
        jHand["yaw"] = direction.yaw() * RAD_TO_DEG;

		// Get how confident we are with a given hand pose
		jHand["confidence"] = hand.confidence();

//#ifdef USE_LEAP_ARM
//        if(this->mConfig.isLoggingArm()) {
//            // Get the Arm bone
//            Arm arm = hand.arm();
//            QJsonObject jArm;
//            jArm["direction"] = this->serializeVector(arm.direction());
//            jArm["wrist"] = this->serializeVector(arm.wristPosition());
//            jArm["elbow"] = this->serializeVector(arm.elbowPosition());

//            jHand["arm"] = jArm;
//        }
//#endif
        // Get fingers
        const FingerList fingers = hand.fingers();
        QJsonArray fingerList;
        for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
            QJsonObject jFinger;
            const Finger finger = *fl;
            jFinger["id"] = finger.id();

            if(this->mConfig.isLoggingName())
                jFinger["name"] = fingerNames[finger.type()].c_str();

            if(this->mConfig.isLoggingFingerDimensions()) {
                jFinger["length"] = finger.length();
                jFinger["width"] = finger.width();
            	jFinger["isExtended"] = finger.isExtended() ? 1 : 0;
            }

            // Get finger bones
            QJsonArray boneList;
            for (int b = 0; b < 4; ++b) {
                QJsonObject jBone;
                Bone::Type boneType = static_cast<Bone::Type>(b);
                Bone bone = finger.bone(boneType);
                jBone["id"] = b;

                if(this->mConfig.isLoggingName())
                    jBone["name"] = boneNames[boneType].c_str();

                if(this->mConfig.isLoggingStart())
                    jBone["start"] = this->serializeVector(bone.prevJoint());

                if(this->mConfig.isLoggingEnd())
                    jBone["end"] = this->serializeVector(bone.nextJoint());

                if(this->mConfig.isLoggingDirection())
                    jBone["direction"] = this->serializeVector(bone.direction());
		
		jBone["xBasis"] = this->serializeVector(bone.basis().xBasis);
		jBone["yBasis"] = this->serializeVector(bone.basis().yBasis);
		jBone["zBasis"] = this->serializeVector(bone.basis().zBasis);

                boneList.append(jBone);
            }
            jFinger["bones"] = boneList;
            //jHand["finger_" + QString::number(finger.id())] = jFinger;
            fingerList.append(jFinger);
        }
        jHand["fingers"] = fingerList;
        //obj["hand_" + QString::number(hand.id())] = jHand;
        handList.append(jHand);
    }
    obj["hands"] = handList;

    QJsonDocument doc(obj);
    if(mCounter != 0)
        (*this->mOut)<< ",\n ";

    (*this->mOut)<< doc.toJson(QJsonDocument::Compact);
    ++this->mCounter;
}

void LeapLogger::onFocusGained(const Controller& controller) {
    std::cout << "Focus Gained" << std::endl;
}

void LeapLogger::onFocusLost(const Controller& controller) {
    std::cout << "Focus Lost" << std::endl;
}

void LeapLogger::onDeviceChange(const Controller& controller) {
    std::cout << "Device Changed" << std::endl;
    const DeviceList devices = controller.devices();

    for (int i = 0; i < devices.count(); ++i) {
        std::cout << "id: " << devices[i].toString() << std::endl;
        std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
    }
}

void LeapLogger::onServiceConnect(const Controller& controller) {
    std::cout << "Service Connected" << std::endl;
    emit this->connected();
}

void LeapLogger::onServiceDisconnect(const Controller& controller) {
    std::cout << "Service Disconnected" << std::endl;
    emit this->disconnected();
}
