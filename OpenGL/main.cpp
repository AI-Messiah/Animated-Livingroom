#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"
#include "StatModel.h"
#include "Shader.h"
#include "Texture2D.h"
#include "SnowTexture.h"
#include "MusicPlayer.h"
#include "VolumeControl.h"

int gWindowWidth, gWindowHeight;

GLFWwindow* gWindow = NULL;

glm::vec4 gClearColor(0.23f, 0.38f, 0.47f, 1.0f);

MyCamera MyCam = MyCamera(glm::vec3(0, 1.0f, 0.0f), 0, 0);
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 2.0; // units per second
const float MOUSE_SENSITIVITY = 0.1f;
const float ROT_SPEED = 20.0;

void showFps(double deltaTime);
void update(double elapsedTime);
bool initOpenGL();

using namespace std;

glm::vec2 appRotation = glm::vec2(0, 0);
float viewAng = 45.0;

double seccnt = 0;
int framnum = 0;

float phase = 0;

float emission = 1;

float seconds = 7;
float minutes = 37;
float hours = 1;

float secTime = 0;


int oldState = GLFW_RELEASE;

VolumeControl control;
MusicPlayer player;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//int main()
{
	float lightTime = 0;
	int lightState = 0;

	float normTime = 0;
	int normState = 0;

	if (!initOpenGL()) {
		
		return -1;
	}

	Shader crucifixShader;
	crucifixShader.window = gWindow;
	crucifixShader.load("crucifix");

	Shader volumeShader;
	volumeShader.window = gWindow;
	volumeShader.load("volume");

	Shader lightShader;
	lightShader.window = gWindow;
	lightShader.load("lights");

	Shader shaderBlur;
	shaderBlur.window = gWindow;
	shaderBlur.load("blur");

	Shader blendShader;
	blendShader.window = gWindow;
	blendShader.load("blend");

	Shader treeShader;
	treeShader.window = gWindow;
	treeShader.load("tree");

	Shader cordShader;
	cordShader.window = gWindow;
	cordShader.load("cord");

	Shader starShader;
	starShader.window = gWindow;
	starShader.load("star");

	Shader baseShader;
	baseShader.window = gWindow;
	baseShader.load("base");

	Shader roomShader;
	roomShader.window = gWindow;
	roomShader.load("room");

	Shader teleShader;
	teleShader.window = gWindow;
	teleShader.load("tele");

	Shader standShader;
	standShader.window = gWindow;
	standShader.load("stand");

	Shader couchShader;
	couchShader.window = gWindow;
	couchShader.load("couch");

	Shader catShader;
	catShader.window = gWindow;
	catShader.load("cat");

	Shader curtainShader;
	curtainShader.window = gWindow;
	curtainShader.load("curtain");

	Shader fanShader;
	fanShader.window = gWindow;
	fanShader.load("ceilingfan");

	Shader ceiLightShader;
	ceiLightShader.window = gWindow;
	ceiLightShader.load("ceillight");

	Shader clockShader;
	clockShader.window = gWindow;
	clockShader.load("clock");

	Shader sceneShader;
	sceneShader.window = gWindow;
	sceneShader.load("cubeMap", true);

	Shader fanMapShader;
	fanMapShader.window = gWindow;
	fanMapShader.load("fanMap", true);

	Shader knobShader;
	knobShader.window = gWindow;
	knobShader.load("doorknob");

	Texture2D volumeTex;
	volumeTex.window = gWindow;
	volumeTex.loadTexture("volbard.png");

	Texture2D crucTex[6];
	string texNames1[] = {"FIG_AO.jpg", "FIG_Diffuse.jpg", "FIG_Normal.jpg", "KRZ_AO.jpg", "KRZ_Diffuse.jpg", "KRZ_Normal.jpg"};
	for (int i = 0; i < 6; i++) {
		crucTex[i].window = gWindow;
		crucTex[i].loadTexture(texNames1[i]);
	}

	Texture2D sceneTex;
	sceneTex.window = gWindow;
	sceneTex.loadTexture("windowScene.png");

	Texture2D carpetTex;
	carpetTex.window = gWindow;
	carpetTex.loadTexture("fabric_0026_color_2k.jpg");

	Texture2D carpetNorm;
	carpetNorm.window = gWindow;
	carpetNorm.loadTexture("fabric_0026_normal_opengl_2k.png");

	Texture2D carpetRou;
	carpetRou.window = gWindow;
	carpetRou.loadTexture("fabric_0026_roughness_2k.jpg");

	Texture2D standDif;
	standDif.window = gWindow;
	standDif.loadTexture("Wood066_8K-JPG_Color.jpg");


	Texture2D leatherDif;
	leatherDif.window = gWindow;
	leatherDif.loadTexture("leather_red_03_coll1_4k.png");

	Texture2D leatherNorm;
	leatherNorm.window = gWindow;
	leatherNorm.loadTexture("leather_red_03_nor_gl_4k.png");

	Texture2D leatherRou;
	leatherRou.window = gWindow;
	leatherRou.loadTexture("leather_red_03_rough_4k.png");

	Texture2D legsTex;
	legsTex.window = gWindow;
	legsTex.loadTexture("wood-68.jpg");

	Texture2D catDif;
	catDif.window = gWindow;
	catDif.loadTexture("bae0fc76-28a1-4058-bccf-0b36a6c534ea_basecolor.JPEG");

	Texture2D catMet;
	catMet.window = gWindow;
	catMet.loadTexture("bae0fc76-28a1-4058-bccf-0b36a6c534ea_metallic.JPEG");

	Texture2D catRou;
	catRou.window = gWindow;
	catRou.loadTexture("bae0fc76-28a1-4058-bccf-0b36a6c534ea_roughness.JPEG");

	Texture2D curtainTex;
	curtainTex.window = gWindow;
	curtainTex.loadTexture("vintpat.png");

	Texture2D bladesec;
	bladesec.window = gWindow;
	bladesec.loadTexture("Material.001_diffuse.png");

	Texture2D machsec;
	machsec.window = gWindow;
	machsec.loadTexture("Material.003_diffuse.png");

	Texture2D darksec;
	darksec.window = gWindow;
	darksec.loadTexture("Material.002_diffuse.png");

	string texNames[] = { "Knob_baseColor.jpeg", "Knob_emissive.jpeg", "Knob_metallicRoughness.png", "Knob_normal.png", "Lamp_Glass_baseColor.jpeg",
							"Lamp_Glass_emissive.jpeg", "Lamp_Glass_metallicRoughness.png", "Lamp_Glass_normal.png", "Lamp_Holder_baseColor.jpeg",
							"Lamp_Holder_emissive.jpeg", "Lamp_Holder_metallicRoughness.png", "Lamp_Holder_normal.png"};

	Texture2D ceiLigntTexs[12];
	for (int i = 0; i < 12; i++) {
		ceiLigntTexs[i].window = gWindow;
		ceiLigntTexs[i].loadTexture(texNames[i]);
	}

	Texture2D faceTex;
	faceTex.window = gWindow;
	faceTex.loadTexture("face.png");

	Texture2D secTex;
	secTex.window = gWindow;
	secTex.loadTexture("sechand.png");

	Texture2D minTex;
	minTex.window = gWindow;
	minTex.loadTexture("minhand.png");

	Texture2D hourTex;
	hourTex.window = gWindow;
	hourTex.loadTexture("hourhand.png");

	SnowTexture snowTex;
	snowTex.window = gWindow;
	snowTex.start();

	StatModel lights;
	lights.window = gWindow;
	lights.isLights = true;
	lights.LoadModel("lights.fbx");

	StatModel tree;
	tree.window = gWindow;
	tree.recHei = true;
	tree.LoadModel("tree.fbx");
	float sh = tree.rHeight;

	StatModel cord;
	cord.window = gWindow;
	cord.LoadModel("cord.fbx");

	StatModel star;
	star.window = gWindow;
	star.isLights = true;
	star.LoadModel("star.fbx");
	vector<glm::vec3> starCents = star.centroids;

	StatModel base;
	base.window = gWindow;
	base.LoadModel("base.fbx");

	StatModel room;
	room.getVals = true;
	room.window = gWindow;
	
	room.LoadModel("Room.fbx");
	float wid = room.wallLen;
	float hei = room.ceilHeight;

	MyCam.setLimits(wid, hei);

	StatModel teleMod;
	teleMod.window = gWindow;
	teleMod.LoadModel("television.fbx");

	StatModel standMod;
	standMod.window = gWindow;
	standMod.LoadModel("tvstand.fbx");

	StatModel couchMod;
	couchMod.window = gWindow;
	couchMod.LoadModel("Couch.fbx");

	StatModel catMod;
	catMod.window = gWindow;
	catMod.LoadModel("cat.fbx");

	StatModel curtainMod;
	curtainMod.window = gWindow;
	curtainMod.LoadModel("curtain.fbx");

	StatModel fanMod;
	fanMod.window = gWindow;
	fanMod.LoadModel("ceilingfan.fbx");

	StatModel ceilLightMod;
	ceilLightMod.window = gWindow;
	ceilLightMod.LoadModel("ceilinglight.fbx");
	

	StatModel clockMod;
	clockMod.window = gWindow;
	clockMod.LoadModel("clock.fbx");

	StatModel doorknob;
	doorknob.window = gWindow;
	doorknob.LoadModel("doorknob.fbx");

	StatModel crucifixMod;
	crucifixMod.window = gWindow;
	crucifixMod.LoadModel("crucifix.fbx");

	
	player.window = gWindow;

	control.init();

	vector<glm::vec3> treeCents = lights.centroids;

	GLuint lVAO, lVBO;

	float vertices[6][4] = {
				{ -1,     1,     0.0f, 1.0f },
				{ -1,     -1,              0.0f, 0.0f },
				{ 1, -1,          1.0f, 0.0f },

				{ -1,     1,     0.0f, 1.0f },
				{ 1, -1,          1.0f, 0.0f },
				{ 1, 1, 1.0f, 1.0f }
	};
	glGenVertexArrays(1, &lVAO);
	glGenBuffers(1, &lVBO);

	glBindVertexArray(lVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		treeCents.size() * sizeof(glm::vec3),
		&treeCents[0],
		GL_STATIC_DRAW);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);


	glm::vec3 lightPos = glm::vec3(0, 4.1, 0);

	unsigned int hdrFBO;
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	unsigned int colorBuffers[2];
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, gWindowWidth, gWindowHeight, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
		);
	}

	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, gWindowWidth, gWindowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffers[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffers[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, gWindowWidth, gWindowHeight, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i], 0
		);
	}

	float nearPlane = 0.1f;
	float farPlane = 10.6;

	glm::mat4 shadowProj = glm::perspective(
		glm::radians(90.0f),
		1.0f,
		nearPlane,
		farPlane
	);

	std::vector<glm::mat4> shadowTransforms;

	shadowTransforms.push_back(
		shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0))
	);

	shadowTransforms.push_back(
		shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0))
	);

	shadowTransforms.push_back(
		shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1))
	);

	shadowTransforms.push_back(
		shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1))
	);

	shadowTransforms.push_back(
		shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0))
	);

	shadowTransforms.push_back(
		shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0))
	);

	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);

	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	

	double lastTime = glfwGetTime();
	double deltaTime;

	player.start();

	while (!glfwWindowShouldClose(gWindow))
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		phase += deltaTime * 1;
		if (phase >= 6.2831853) phase -= 6.2831853;

		secTime += deltaTime;
		if (secTime >= 1) {
			secTime -= 1;
			seconds++;
			if (seconds > 59) {
				seconds = 0;
				minutes++;
				if (minutes > 59) {
					minutes = 0;
					hours++;
					if (hours > 11) {
						hours = 0;
					}
				}
			}
		}

		glfwPollEvents();
		update(deltaTime);
		showFps(deltaTime);
		
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClearDepth(1.0);
		glClear(GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		sceneShader.use(); // this is a shader for shadows

		for (int i = 0; i < 6; ++i)
			sceneShader.setUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

		sceneShader.setUniform("lightPos", lightPos);
		sceneShader.setUniform("farPlane", farPlane);

		glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(-5.894, 1.123, 5.775)) * glm::rotate(glm::mat4(1), glm::radians(135.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		sceneShader.setUniform("model", model);
		standMod.Draw();

		model = glm::translate(glm::mat4(1), glm::vec3(-5.894, .899, 5.775)) * glm::rotate(glm::mat4(1), glm::radians(135.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		sceneShader.setUniform("model", model);
		teleMod.Draw();

		model = glm::translate(glm::mat4(1), glm::vec3(0, .067, 1.124)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.5, 1.5, 1.5));
		sceneShader.setUniform("model", model);

		couchMod.Draw();

		model = glm::translate(glm::mat4(1), glm::vec3(5.3, 0, 5.3)) * glm::rotate(glm::mat4(1), glm::radians(227.143f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(.817, .817, .817));

		sceneShader.setUniform("model", model);
		if (lightState == 0) tree.Draw();

		fanMapShader.use();
		for (int i = 0; i < 6; ++i)
			fanMapShader.setUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

		fanMapShader.setUniform("lightPos", lightPos);
		fanMapShader.setUniform("farPlane", farPlane);

		// bind here
		model = glm::translate(glm::mat4(1), glm::vec3(2.832, 1.461, 0)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));


		fanMapShader.setUniform("model", model);

		model = glm::translate(glm::mat4(1), glm::vec3(2.832, 1.461, 0)) * glm::rotate(glm::mat4(1), phase, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		

		fanMapShader.setUniform("rotmod", model);
		
		fanMod.Draw();

		model = glm::translate(glm::mat4(1), glm::vec3(-2.832, 1.461, 0)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		

		fanMapShader.setUniform("model", model);
		model = glm::translate(glm::mat4(1), glm::vec3(-2.832, 1.461, 0)) * glm::rotate(glm::mat4(1), phase, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		

		fanMapShader.setUniform("rotmod", model);
		

		fanMod.Draw();


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		glViewport(0, 0, gWindowWidth, gWindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		model = glm::translate(glm::mat4(1), glm::vec3(-5.894, 1.123, 5.775)) * glm::rotate(glm::mat4(1), glm::radians(135.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));

		glm::mat4 mvp = MyCam.getMVP(model);

		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

		

		model = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.5, 1.5, 1.5));

		glm::mat4 vp1 = MyCam.getVP();

		glm::vec3 viewPos = MyCam.campos;		

		//farPlane = 9;

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		roomShader.use();
		roomShader.setUniform("vp", vp1);
		roomShader.setUniform("model", model);
		roomShader.setUniform("lightPos", lightPos);
		roomShader.setUniform("viewPos", viewPos);
		roomShader.setUniform("farPlane", farPlane);
		roomShader.setUniformSampler("Texture1", 0);
		roomShader.setUniformSampler("Texture2", 1);
		roomShader.setUniformSampler("Texture3", 2);
		roomShader.setUniformSampler("Texture4", 3);
		roomShader.setUniformSampler("depthCubemap", 4);
		
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

		carpetRou.bind(3);
		carpetNorm.bind(2);
		carpetTex.bind(1);
		
		sceneTex.bind(0);
		room.Draw();
		carpetTex.unbind(1);
		
		sceneTex.unbind(0);
		carpetRou.bind(3);
		carpetNorm.bind(2);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		model = glm::translate(glm::mat4(1), glm::vec3(-5.894, 1.123, 5.775)) * glm::rotate(glm::mat4(1), glm::radians(135.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));

		mvp = MyCam.getMVP(model);

		standShader.use();
		standShader.setUniform("mvp", mvp);
		standShader.setUniformSampler("Texture1", 0);
		standShader.setUniform("lightPos", lightPos);
		standShader.setUniform("viewPos", viewPos);
		standShader.setUniform("model", model);

		standDif.bind(0);
		standMod.Draw();
		standDif.unbind(0);


		model = glm::translate(glm::mat4(1), glm::vec3(-5.894, .899, 5.775)) * glm::rotate(glm::mat4(1), glm::radians(135.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));

		mvp = MyCam.getMVP(model);

		teleShader.use();
		teleShader.setUniform("mvp", mvp);
		teleShader.setUniformSampler("Texture1", 0);
		teleShader.setUniform("lightPos", lightPos);
		teleShader.setUniform("viewPos", viewPos);
		teleShader.setUniform("model", model);

		snowTex.update(deltaTime);
		snowTex.bind(0);
		teleMod.Draw();
		snowTex.unbind(0);


		model = glm::translate(glm::mat4(1), glm::vec3(0, .067, 1.124)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.5, 1.5, 1.5));

		mvp = MyCam.getMVP(model);

		couchShader.use();
		couchShader.setUniform("mvp", mvp);
		couchShader.setUniformSampler("Texture1", 0);
		couchShader.setUniformSampler("Texture2", 1);
		couchShader.setUniformSampler("Texture3", 2);
		couchShader.setUniformSampler("Texture4", 3);
		couchShader.setUniform("lightPos", lightPos);
		couchShader.setUniform("viewPos", viewPos);
		couchShader.setUniform("model", model);

		leatherRou.bind(3);
		leatherNorm.bind(2);
		leatherDif.bind(0);
		legsTex.bind(1);
		couchMod.Draw();
		leatherDif.unbind(0);
		legsTex.unbind(1);
		leatherRou.unbind(3);
		leatherNorm.unbind(2);

		model = glm::translate(glm::mat4(1), glm::vec3(-.255, .067, 1.124)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.5, 1.5, 1.5));

		mvp = MyCam.getMVP(model);

		catShader.use();
		catShader.setUniform("mvp", mvp);
		catShader.setUniformSampler("Texture1", 0);
		catShader.setUniformSampler("Texture2", 1);
		catShader.setUniformSampler("Texture3", 2);
		catShader.setUniform("lightPos", lightPos);
		catShader.setUniform("viewPos", viewPos);
		catShader.setUniform("model", model);

		catRou.bind(2);
		catMet.bind(1);
		catDif.bind(0);
		catMod.Draw();
		catDif.unbind(0);
		catRou.unbind(2);
		catMet.unbind(1);

		model = glm::translate(glm::mat4(1), glm::vec3(2.377, 2.562, 6.542)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.5, 1.5, 1.5));
		mvp = MyCam.getMVP(model);

		curtainShader.use();		
		curtainShader.setUniform("time", phase);
		curtainShader.setUniformSampler("Texture1", 0);
		curtainShader.setUniform("lightPos", lightPos);
		curtainShader.setUniform("viewPos", viewPos);
		curtainShader.setUniform("model", model);

		curtainShader.setUniformI("side", 0);
		curtainShader.setUniform("mvp", mvp);

		curtainTex.bind(0);		
		curtainMod.Draw();

		model = glm::translate(glm::mat4(1), glm::vec3(-2.377, 2.562, 6.542)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.5, 1.5, 1.5));
		mvp = MyCam.getMVP(model);

		curtainShader.setUniformI("side", 1);
		curtainShader.setUniform("mvp", mvp);

		curtainMod.Draw();
		curtainTex.unbind(0);

		fanShader.use();
		fanShader.setUniformSampler("Texture1", 0);
		fanShader.setUniformSampler("Texture2", 1);
		fanShader.setUniformSampler("Texture3", 2);
		fanShader.setUniform("lightPos", lightPos);
		fanShader.setUniform("viewPos", viewPos);
		

		model = glm::translate(glm::mat4(1), glm::vec3(2.832, 1.461, 0)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		mvp = MyCam.getMVP(model);

		fanShader.setUniform("model", model);
		fanShader.setUniform("mvp", mvp);

		model = glm::translate(glm::mat4(1), glm::vec3(2.832, 1.461, 0)) * glm::rotate(glm::mat4(1), phase, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		mvp = MyCam.getMVP(model);

		fanShader.setUniform("rotmodel", model);
		fanShader.setUniform("rotmvp", mvp);

		bladesec.bind(0);
		machsec.bind(1);
		darksec.bind(2);
		fanMod.Draw();

		model = glm::translate(glm::mat4(1), glm::vec3(-2.832, 1.461, 0)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		mvp = MyCam.getMVP(model);

		fanShader.setUniform("model", model);
		fanShader.setUniform("mvp", mvp);

		model = glm::translate(glm::mat4(1), glm::vec3(-2.832, 1.461, 0)) * glm::rotate(glm::mat4(1), phase, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		mvp = MyCam.getMVP(model);

		fanShader.setUniform("rotmodel", model);
		fanShader.setUniform("rotmvp", mvp);

		fanMod.Draw();

		bladesec.unbind(0);
		machsec.unbind(1);
		darksec.unbind(2);

		//model = glm::translate(glm::mat4(1), glm::vec3(0, transy, 0));
		//mvp = MyCam.getMVP(model);

		model = glm::translate(glm::mat4(1), glm::vec3(6.667, 2, -3.881)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.233, 1.233, 1.233));
		mvp = MyCam.getMVP(model);
		knobShader.use();
		knobShader.setUniform("model", model);
		knobShader.setUniform("mvp", mvp);
		knobShader.setUniform("lightPos", lightPos);
		knobShader.setUniform("viewPos", viewPos);

		doorknob.Draw();

		model = glm::translate(glm::mat4(1), glm::vec3(6.786, 1.384, -6.065)) * glm::rotate(glm::mat4(1), glm::radians(274.286f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(1.785f), glm::vec3(0, 0, 1)) * glm::rotate(glm::mat4(1), glm::radians(270.595f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(1.394, 1.394, 1.394));
		mvp = MyCam.getMVP(model);

		glm::vec3 campos = MyCam.campos;
		glm::mat4 vp = MyCam.getVP();

		normTime += deltaTime;
		if (normTime >= 5) {
			normTime = 0;
			//normState = 1 - normState;
		}

		crucifixShader.use();
		crucifixShader.setUniform("vp", vp);
		crucifixShader.setUniform("model", model);
		crucifixShader.setUniform("viewPos", campos);
		crucifixShader.setUniform("lightPos", glm::vec3(0, 1.5, 0));
		crucifixShader.setUniformI("normState", normState);
		for (int i = 0; i < 6; i++) {
			string iname = "Texture" + std::to_string(i + 1);
			crucifixShader.setUniformSampler(iname.c_str(), i);
			crucTex[i].bind(i);
		}

		crucifixMod.Draw();

		for (int i = 0; i < 6; i++) crucTex[i].unbind(i);

		model = glm::translate(glm::mat4(1), glm::vec3(-4.045, 3, 6.737)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 0, 1)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(.3, .3, .3));

		mvp = MyCam.getMVP(model);

		clockShader.use();
		clockShader.setUniform("mvp", mvp);
		catShader.setUniform("lightPos", lightPos);
		catShader.setUniform("viewPos", viewPos);
		catShader.setUniform("model", model);

		float hndrot = seconds * 6.0f;

		model = glm::translate(glm::mat4(1), glm::vec3(-4.045, 3, 6.737)) * glm::rotate(glm::mat4(1), glm::radians(180.0f + hndrot), glm::vec3(0, 0, 1)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(.3, .3, .3));

		mvp = MyCam.getMVP(model);

		clockShader.setUniform("secmodel", model);
		clockShader.setUniform("secmvp", mvp);

		float amins = seconds / 60.0f + minutes;

		hndrot = amins * 6.0f;
		model = glm::translate(glm::mat4(1), glm::vec3(-4.045, 3, 6.737)) * glm::rotate(glm::mat4(1), glm::radians(180.0f + hndrot), glm::vec3(0, 0, 1)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(.3, .3, .3));

		mvp = MyCam.getMVP(model);

		clockShader.setUniform("minmodel", model);
		clockShader.setUniform("minmvp", mvp);

		float ahours = amins / 60.0f + hours;
		hndrot = ahours * 30.0f;

		model = glm::translate(glm::mat4(1), glm::vec3(-4.045, 3, 6.737)) * glm::rotate(glm::mat4(1), glm::radians(180.0f + hndrot), glm::vec3(0, 0, 1)) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(0.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(.3, .3, .3));

		mvp = MyCam.getMVP(model);

		clockShader.setUniform("hourmodel", model);
		clockShader.setUniform("hourmvp", mvp);
		for (int i = 0; i < 4; i++) {
			string unifName = "Texture" + to_string(i + 1);
			clockShader.setUniformSampler(unifName.c_str(), i);
		}

		faceTex.bind(0);
		secTex.bind(1);
		minTex.bind(2);
		hourTex.bind(3);

		clockMod.Draw();

		faceTex.unbind(0);
		secTex.unbind(1);
		minTex.unbind(2);
		hourTex.unbind(3);

		model = glm::translate(glm::mat4(1), glm::vec3(5.3, 0, 5.3)) * glm::rotate(glm::mat4(1), glm::radians(227.143f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0)) * glm::scale(glm::mat4(1), glm::vec3(.817, .817, .817));

		mvp = MyCam.getMVP(model);

		cordShader.use();
		cordShader.setUniform("mvp", mvp);
		cordShader.setUniformI("lightState", lightState);

		cord.Draw();

		

		treeShader.use();
		treeShader.setUniform("mvp", mvp);
		treeShader.setUniformI("lightState", lightState);

		

		treeShader.bindSSBO(ssbo);
		treeShader.setUniform("viewPos", MyCam.getPosition());
		treeShader.setUniform("starCent", starCents[0]);		
		
		tree.Draw();

		

		baseShader.use();
		baseShader.setUniform("mvp", mvp);

		base.Draw();

		//if (lightState == 1) glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

		

		lightShader.use();
		lightShader.setUniform("vp", vp);
		lightShader.setUniform("model", model);
		lightShader.setUniformI("lightState", lightState);
		lightShader.setUniform("viewPos", campos);
		lights.Draw();
		
		starShader.use();
		starShader.setUniform("mvp", mvp);

		star.Draw();
		glm::vec3 curLightPos = glm::vec3(0, 1.532, 0);
		model = glm::translate(glm::mat4(1), curLightPos) * glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), glm::radians(270.0f), glm::vec3(1, 0, 0));
		
		ceiLightShader.use();
		ceiLightShader.setUniform("vp", vp);
		ceiLightShader.setUniform("model", model);
		ceiLightShader.setUniform("cameraPos", campos);		
		ceiLightShader.setUniform("lightPos", curLightPos);

		for (int i = 0; i < 12; i++) {
			string unifName = "Texture" + to_string(i + 1);
			ceiLightShader.setUniformSampler(unifName.c_str(), i);
			ceiLigntTexs[i].bind(i);
		}

		ceilLightMod.Draw();
		for (int i = 0; i < 12; i++) ceiLigntTexs[i].unbind(i);

		lightTime += deltaTime;
		if (lightTime >= 1) {
			lightTime -= 1;
			lightState = 1 - lightState;
		}

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		bool horizontal = true, first_iteration = true;
		int amount = 10;
		shaderBlur.use();
		shaderBlur.setUniformSampler("image", 0);
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			shaderBlur.setUniformI("horizontal", horizontal);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]
			);			

			glBindVertexArray(lVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		blendShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]);
		
		blendShader.setUniformSampler("scene", 0);
		blendShader.setUniformSampler("bloomBlur", 1);
		

		glBindVertexArray(lVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		volumeShader.use();
		volumeShader.setUniformSampler("Texture1", 0);

		volumeTex.bind(0);
		control.draw();
		volumeTex.unbind(0);

		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(gWindow);
	}

	glDeleteVertexArrays(1, &lVAO);
	glDeleteBuffers(1, &lVBO);

	glfwTerminate();

	return 0;

}

void showFps(double deltaTime)
{
	seccnt += deltaTime;
	framnum++;
	if (seccnt >= 1) {
		seccnt -= 1;
		string num = std::to_string(framnum);
		glfwSetWindowTitle(gWindow, num.c_str());
		framnum = 0;
	}
}

void update(double deltaTime)
{
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
		MyCam.move(MOVE_SPEED * (float)deltaTime * MyCam.getLook());
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
		MyCam.move(MOVE_SPEED * (float)deltaTime * -MyCam.getLook());

	// Strafe left/right
	if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
		MyCam.move(MOVE_SPEED * (float)deltaTime * -MyCam.getRight());
	else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
		MyCam.move(MOVE_SPEED * (float)deltaTime * MyCam.getRight());

	// Up/down
	if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
		MyCam.move(MOVE_SPEED * (float)deltaTime * MyCam.getUp());
	else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
		MyCam.move(MOVE_SPEED * (float)deltaTime * -MyCam.getUp());

	if (glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
		appRotation.y += ROT_SPEED * deltaTime;
		MyCam.setRotation(appRotation);
	}
	else if (glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS){
		appRotation.y -= ROT_SPEED * deltaTime;
		MyCam.setRotation(appRotation);
	}

	if (glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS) {
		appRotation.x += ROT_SPEED * deltaTime;
		MyCam.setRotation(appRotation);
	}
	else if (glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
		appRotation.x -= ROT_SPEED * deltaTime;
		MyCam.setRotation(appRotation);
	}

	if (glfwGetKey(gWindow, GLFW_KEY_O) == GLFW_PRESS) {
		viewAng -= ROT_SPEED * deltaTime;
		if (viewAng < 5) viewAng = 5;
		MyCam.setFov(viewAng);
	}
	else if (glfwGetKey(gWindow, GLFW_KEY_P) == GLFW_PRESS) {
		viewAng += ROT_SPEED * deltaTime;
		if (viewAng > 90) viewAng = 90;
		MyCam.setFov(viewAng);
	}

	if (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(gWindow, true);

	double xpos, ypos;
	glfwGetCursorPos(gWindow, &xpos, &ypos);
	xpos /= (float)gWindowWidth;
	xpos *= 2;
	xpos -= 1;
	ypos /= (float)gWindowHeight;
	ypos *= 2;
	ypos -= 1;
	ypos *= -1;

	int state = glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		control.drag(xpos, ypos);
		player.setVolume(control.volume);
		oldState = GLFW_PRESS;
	}
	else if (oldState == GLFW_PRESS) {
		control.reset();
		oldState = GLFW_RELEASE;
	}
}


bool initOpenGL()
{
	if (!glfwInit())
	{
		InitFailure("GLFW initialization failed");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	gWindowWidth = 1536;
	gWindowHeight = 864;

	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, "", NULL, NULL);

	glfwWindowHint(GLFW_FOCUSED, GL_TRUE);

	glfwRestoreWindow(gWindow);

	if (gWindow == NULL)
	{
		InitFailure("Failed to create GLFW window");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(gWindow);


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		InitFailure("Failed to initialize GLEW");
		return false;
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glEnable(GL_DEPTH_TEST);
	

	return true;
}