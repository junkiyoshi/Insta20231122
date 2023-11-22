#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetColor(0);

	ofEnableDepthTest();
	ofSetLineWidth(1);

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->line.clear();

	auto noise_param = glm::vec3(ofRandom(360), ofRandom(360), ofRandom(360));

	for (int radius = 50; radius <= 300; radius += 50) {

		auto rotation = glm::vec3(
			ofMap(ofNoise(noise_param.x, radius * 0.001 - ofGetFrameNum() * 0.005), 0, 1, -180, 180),
			ofMap(ofNoise(noise_param.y, radius * 0.001 - ofGetFrameNum() * 0.005), 0, 1, -180, 180),
			ofMap(ofNoise(noise_param.z, radius * 0.001 - ofGetFrameNum() * 0.005), 0, 1, -180, 180));

		auto rotation_x = glm::rotate(glm::mat4(), rotation.x * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
		auto rotation_y = glm::rotate(glm::mat4(), rotation.y * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
		auto rotation_z = glm::rotate(glm::mat4(), rotation.z * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

		int start_index = this->line.getNumVertices();
		int index = start_index;
		for (int deg = 0; deg < 360; deg += 1) {

			auto vertex = glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0);

			if (radius % 150 != 0) {

				vertex = glm::vec4(vertex, 0) * rotation_x * rotation_y;
			}

			this->line.addVertex(vertex);

			if (deg > 0) {

				this->line.addIndex(index - 1);
				this->line.addIndex(index);
			}

			index++;
		}

		this->line.addIndex(this->line.getNumVertices() - 1);
		this->line.addIndex(start_index);
	}

	int index_span = 30;
	for (int index = 0; index < this->line.getNumVertices() - 360; index += index_span) {

		this->line.addIndex(index);
		this->line.addIndex(index + 360);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	this->line.drawWireframe();

	this->cam.end();

	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}