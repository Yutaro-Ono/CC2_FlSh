#include "Renderer.h"
#include "GameMain.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshGpmesh.h"
#include "MeshObj.h"
#include "Skeleton.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "SkeletalMeshComponent.h"
#include "CubeMapComponent.h"
#include "EnvironmentMapComponent.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Animation.h"
#include "ParticleManager.h"
#include "UIScreen.h"
#include "PostEffect.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "WorldSpaceUI.h"
#include "ShadowMap.h"
#include "CameraComponent.h"
#include "RenderBloom.h"
#include "ForwardRenderer.h"
#include "DefferedRenderer.h"
#include "ShaderManager.h"
#include "../imgui/imconfig.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_sdl.h"
#include <iostream>
#include "DirectionalLight.h"
#include "EffekseerEffect.h"
#include "SkyBox.h"
#include "Debugger.h"

// �R���X�g���N�^
Renderer::Renderer()
	:m_window(nullptr)
	,m_SDLRenderer(nullptr)
	,m_context(0)
	,m_uboMatrices(0)
	,m_postEffect(nullptr)
	,m_bloom(nullptr)
	,m_switchShader(0)
	,m_cameraPos(Vector3::Zero)
	,m_fRenderer(nullptr)
	,m_dRenderer(nullptr)
	,m_mapHUD(nullptr)
	,m_shaderManager(nullptr)
{
	m_enableBloom = GAME_CONFIG->GetEnableBloom();
	m_visualizeNormal = false;

	// �`����@�̐ݒ�
	if (GAME_CONFIG->GetEnableDeferred())
	{
		m_renderMode = RENDER_MODE::DEFFERED;
	}
}

// �f�X�g���N�^
Renderer::~Renderer()
{
}

// �e�평�������� (GL SDL)
bool Renderer::Initialize(int in_screenW, int in_screenH, bool in_full)
{
	// ��ʃT�C�Y��ݒ�
	m_screenWidth = in_screenW;
	m_screenHeight = in_screenH;

	//-----------------------------------------------------------------+
    // OpenGL�̑����ݒ�
    //-----------------------------------------------------------------+
    // OpenGL�R�A�v���t�@�C�����g�p����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// �w�� -> OpenGL/ver4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);     // ���W���[�o�[�W����
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);     // �}�C�i�[�o�[�W����
	// RGBA�e�`�����l����8bit�̃J���[�o�b�t�@���w�肷��
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);                  // R
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);                // G
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);                 // B
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);                // A
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// �_�u���o�b�t�@��L���ɂ���
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// �n�[�h�E�F�A�A�N�Z�����[�V�������g�p����
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// SDL�E�B���h�E�̍쐬
	m_window = SDL_CreateWindow
	(
		"ProjectTPL",                                     // �E�B���h�E�̖���
		0,                                                // x���W�̃E�B���h�E�`�挴�_
		0,                                                // y���W�̃E�B���h�E�`�挴�_
		m_screenWidth,                                    // ��ʂ̉���
		m_screenHeight,                                   // ��ʂ̏c��
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	// �E�B���h�E�̍쐬�Ɏ��s������
	if (!m_window)
	{
		std::cout << "FAILED::SDL::CreateWindow::" << SDL_GetError() << std::endl;
		return false;
	}
	// �t���X�N���[��������
	if (in_full)
	{
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	// wminfo�\����
	SDL_SysWMinfo wminfo;
	SDL_VERSION(&wminfo.version);
	SDL_GetWindowWMInfo(m_window, &wminfo);
	m_hwnd = wminfo.info.win.window;
	
    //----------------------------------------------------------------+
    // OpenGL�R���e�L�X�g�ݒ�
    //----------------------------------------------------------------+
    // OpenGL�R���e�L�X�g�𐶐�
	m_context = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_context);

    //----------------------------------------------------------------+
    // GLEW������
    //----------------------------------------------------------------+
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "FAILED::GLEW::glewInit()" << std::endl;
		return false;
	}
	// ����̃v���b�g�z�[���ł́AGLEW�����Q�ȃG���[�R�[�h��f���̂ŃN���A���Ă���
	glGetError();


	// �r���[�|�[�g�ݒ�
	glViewport(0, 0, m_screenWidth, m_screenHeight);

	//-----------------------------------------------------------------+
	// SDL�����_���[�̍쐬
	//-----------------------------------------------------------------+
	m_SDLRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_SDLRenderer)
	{
		std::cout << "FAILED::SDL::CreateRenderer::" << SDL_GetError() << std::endl;
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		std::cout << "FAILED::SDL_Image::INIT_PNG::" << SDL_GetError() << std::endl;
		return false;
	}

	//--------------------------------------------+
	// Effekseer������
	//--------------------------------------------+
	/*
	m_effekseerRenderer = ::EffekseerRendererGL::Renderer::Create(8000, EffekseerRendererGL::OpenGLDeviceType::OpenGL3);
	m_effekseerManager = ::Effekseer::Manager::Create(8000);
	// �`�惂�W���[���쐬
	m_effekseerManager->SetSpriteRenderer(m_effekseerRenderer->CreateSpriteRenderer());
	m_effekseerManager->SetRibbonRenderer(m_effekseerRenderer->CreateRibbonRenderer());
	m_effekseerManager->SetRingRenderer(m_effekseerRenderer->CreateRingRenderer());
	m_effekseerManager->SetTrackRenderer(m_effekseerRenderer->CreateTrackRenderer());
	m_effekseerManager->SetModelRenderer(m_effekseerRenderer->CreateModelRenderer());
	// Effekseer�p�̃e�N�X�`���E���f���E�}�e���A�����[�_�[
	m_effekseerManager->SetTextureLoader(m_effekseerRenderer->CreateTextureLoader());
	m_effekseerManager->SetModelLoader(m_effekseerRenderer->CreateModelLoader());
	m_effekseerManager->SetMaterialLoader(m_effekseerRenderer->CreateMaterialLoader());
	*/


	// �������ɐ���
	return true;
}

bool Renderer::Load()
{
	//------------------------------------------------------------------+
    // �p�[�e�B�N���֘A
    //------------------------------------------------------------------+
    // �p�[�e�B�N���}�l�[�W���[�쐬
	m_particleManager = new ParticleManager;

	//------------------------------------------------------------------+
	// ���_�z��I�u�W�F�N�g�n�̐���
	//------------------------------------------------------------------+
	// �p�[�e�B�N���p
	m_particleVerts = new VertexArray();
	m_particleVerts->CreateSpriteVerts();
	// �X�v���C�g�p
	m_spriteVerts = new VertexArray();
	m_spriteVerts->CreateSpriteVerts();
	// �L���[�u�}�b�v�p
	m_cubeVerts = new VertexArray();
	m_cubeVerts->CreateCubeVerts();
	// ��ʏo�͗p
	m_screenVerts = new VertexArray();
	m_screenVerts->CreateScreenVerts();



	//--------------------------------------------+
	// uniform Buffer Object
	//--------------------------------------------+
	CreateUBOs();

	//------------------------------------------------------------------+
    // �|�X�g�G�t�F�N�g
    //------------------------------------------------------------------+
	m_postEffect = new PostEffect();
	//m_postEffect->Load();

	//--------------------------------------------+
	// �V���h�E�}�b�v
	//--------------------------------------------+
	m_shadowMap = new ShadowMap();

	//--------------------------------------------+
	// Bloom
	//--------------------------------------------+
	m_bloom = new RenderBloom();

	//--------------------------------------------+
	// Forward�EDeferedRenderer
	//--------------------------------------------+
	m_fRenderer = new ForwardRenderer(this);
	m_dRenderer = new DefferedRenderer(this);
	if (!m_dRenderer->Initialize())
	{
		return false;
	}

	//--------------------------------------------+
    // �V�F�[�_�[�Ǘ��N���X
    //--------------------------------------------+
	m_shaderManager = new ShaderManager();
	if (!m_shaderManager->CreateShaders())
	{
		std::cout << "ERROR::ShaderManager::Func::CreateShaders()" << std::endl;
		return false;
	}

	// �f�B���N�V���i�����C�g
	m_dirLight = new DirectionalLight();

	// ���̃X�J�C�{�b�N�X����(�L���[�u�}�b�v)
	SkyBox* skybox = new SkyBox(ENVIRONMENT_TYPE::NIGHT);
	m_activeSkyBox = skybox->GetCubeMapComp();

	return true;
}

void Renderer::Delete()
{
	// �e�N�X�`���̉��
	for (auto i : m_textures)
	{
		printf("Textures Release : %s\n", i.first.c_str());
		i.second->Delete();
		delete i.second;
	}
	m_textures.clear();
	// �p�[�e�B�N���}�l�[�W���[�j��
	delete m_particleManager;

	// ���b�V���̉��
	for (auto i : m_meshes)
	{
		printf("Meshes Release : %s\n", i.first.c_str());
		i.second->Delete();
		delete i.second;
	}
	m_meshes.clear();
	// �X�P���g���̔j��
	for (auto sk : m_skeletons)
	{
		delete sk.second;
	}
	m_skeletons.clear();
	// �A�j���[�V�����̔j��
	for (auto anim : m_animations)
	{
		delete anim.second;
	}
	m_animations.clear();

	// �ԃ��b�V���z��̉��
	for (auto car : m_carMeshComponents)
	{
		delete car;
	}
	m_carMeshComponents.clear();
	// ���C�g�O���X�z��̉��
	for (auto light : m_lightGlassComponents)
	{
		delete light;
	}
	m_lightGlassComponents.clear();
	// ���}�b�v�I�u�W�F�N�g�z��̉��
	for (auto env : m_envMeshComponents)
	{
		delete env;
	}
	m_envMeshComponents.clear();

	// �X�v���C�g�̉��
	for (auto sprite : m_worldSprites)
	{
		delete sprite;
	}
	m_worldSprites.clear();
	// �X�v���C�g�̉��
	for (auto sprite : m_spriteComponents)
	{
		delete sprite;
	}
	m_spriteComponents.clear();
	// �|�C���g���C�g�z��̉��
	for (auto light : m_pointLights)
	{
		delete light;
	}
	// �X�|�b�g���C�g�z��̉��
	for (auto spLight : m_spotLights)
	{
		delete spLight;
	}

	// �V�F�[�_�[�Ǘ��N���X�̉��
	delete m_shaderManager;

	delete m_dirLight;
	delete m_spriteVerts;
	delete m_cubeVerts;
	delete m_fRenderer;
	delete m_dRenderer;
	delete m_postEffect;
	delete m_shadowMap;
	delete m_bloom;

	/*
	// Effekseer�֘A�̉��
	m_effekseerManager.Reset();
	m_effekseerRenderer.Reset();
	*/

	// �R���e�L�X�g�̔j��
	SDL_GL_DeleteContext(m_context);
	// �E�B���h�E�̔j��
	SDL_DestroyWindow(m_window);
}

// �`�揈��
void Renderer::Draw()
{

	//------------------------------------------------+
	// �����_�����O (Forward or Deffered)
	//------------------------------------------------+
	// ���ʏ���
	// �f�B���N�V���i�����C�g�̍X�V
	m_dirLight->Update();
	// uniform�o�b�t�@�֋��ʏ����i�[����
	UpdateUBO();

	// �V���h�E�`��p�̐[�x�}�b�v�Ƀ��C�g���_���猩����Ԃŏ�������
	//m_shadowMap->RenderDepthMapFromLightView(m_meshComponents, m_skeletalMeshComponents, m_carMeshComponents);
	// �������番��
	if (m_renderMode == RENDER_MODE::FORWARD)
	{
		m_fRenderer->Draw();
	}
	else if (m_renderMode == RENDER_MODE::DEFFERED)
	{
		m_dRenderer->Draw();
	}

#ifdef _DEBUG

	// �f�o�b�O�̕`��
	GAME_INSTANCE.DebugRenderer();

#endif

	// ��ʂ̃X���b�v
	SDL_GL_SwapWindow(m_window);
}


void Renderer::AddSprite(SpriteComponent * in_sprite)
{

	int myDrawOrder = in_sprite->GetDrawOrder();
	auto iter = m_spriteComponents.begin();
	for (;
		iter != m_spriteComponents.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// �C�e���[�^�Ŏw�肵���ԍ��ɃX�v���C�g��}��
	m_spriteComponents.insert(iter, in_sprite);
}

// �w�肵���X�v���C�g�̍폜
void Renderer::RemoveSprite(SpriteComponent * in_sprite)
{
	// �C�e���[�^�ō폜�w���̏o���X�v���C�g��T��
	auto iter = std::find(m_spriteComponents.begin(), m_spriteComponents.end(), in_sprite);
	m_spriteComponents.erase(iter);
}

// ���[���h��̃X�v���C�g��z��ɒǉ�
void Renderer::AddSpriteWorld(WorldSpaceUI* in_sprite)
{
	m_worldSprites.push_back(in_sprite);
}

// ���[���h�X�v���C�g�z�񂩂�w�肵���X�v���C�g���폜
void Renderer::RemoveSpriteWorld(WorldSpaceUI* in_sprite)
{
	auto iter = std::find(m_worldSprites.begin(), m_worldSprites.end(), in_sprite);
	m_worldSprites.erase(iter);
}

// ���b�V���R���|�[�l���g�ǉ�
void Renderer::AddMeshComponent(MeshComponent * in_mesh)
{

	if (in_mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(in_mesh);
		m_skeletalMeshComponents.emplace_back(sk);
	}
	else
	{
		m_meshComponents.emplace_back(in_mesh);
	}

}

// ���b�V���R���|�[�l���g�i�[�z�񂩂�폜
void Renderer::RemoveMeshComponent(MeshComponent * in_mesh)
{

	if (in_mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(in_mesh);
		auto iter = std::find(m_skeletalMeshComponents.begin(), m_skeletalMeshComponents.end(), sk);
		m_skeletalMeshComponents.erase(iter);
	}
	else
	{
		auto iter = std::find(m_meshComponents.begin(), m_meshComponents.end(), in_mesh);
		m_meshComponents.erase(iter);
	}

}
/// <summary>
/// �ԃ��b�V���i�[
/// </summary>
/// <param name="in_carMesh">�ǉ�����ԗp���b�V���R���|�[�l���g</param>
void Renderer::AddCarMeshComponent(CarMeshComponent* in_carMesh)
{
	m_carMeshComponents.emplace_back(in_carMesh);
}
/// <summary>
/// �ԃ��b�V���폜
/// </summary>
/// <param name="in_carMesh">�폜����ԗp���b�V���R���|�[�l���g</param>
void Renderer::RemoveCarMeshComponent(CarMeshComponent* in_carMesh)
{
	auto iter = std::find(m_carMeshComponents.begin(), m_carMeshComponents.end(), in_carMesh);
	m_carMeshComponents.erase(iter);
}

// ���}�b�v�I�u�W�F�N�g�z��ւ̒ǉ�
void Renderer::AddEnvironmentComponent(EnvironmentMapComponent* in_envMesh)
{
	m_envMeshComponents.push_back(in_envMesh);
}

// ���}�b�v�I�u�W�F�N�g�z�񂩂�폜
void Renderer::RemoveEnvironmentComponent(EnvironmentMapComponent* in_envMesh)
{
	auto iter = std::find(m_envMeshComponents.begin(), m_envMeshComponents.end(), in_envMesh);
	m_envMeshComponents.erase(iter);
}

void Renderer::AddLightGlassComponent(LightGlassComponent* in_glassMesh)
{
	m_lightGlassComponents.push_back(in_glassMesh);
}

void Renderer::RemoveLightGlassComponent(LightGlassComponent* in_glassMesh)
{
	auto iter = std::find(m_lightGlassComponents.begin(), m_lightGlassComponents.end(), in_glassMesh);
	m_lightGlassComponents.erase(iter);
}

/// <summary>
/// �|�C���g���C�g�R���|�[�l���g�ǉ�
/// �ǉ������ƕ`��ɓK�p�����
/// </summary>
/// <param name="in_pointL">�ǉ�����|�C���g���C�g�N���X</param>
void Renderer::AddPointLightComponent(PointLightComponent* in_pointL)
{
	m_pointLights.push_back(in_pointL);
}

/// <summary>
/// �|�C���g���C�g�R���|�[�l���g�폜
/// </summary>
/// <param name="in_pointL">�폜����|�C���g���C�g�N���X</param>
void Renderer::RemovePointLightComponent(PointLightComponent* in_pointL)
{
	auto iter = std::find(m_pointLights.begin(), m_pointLights.end(), in_pointL);
	m_pointLights.erase(iter);
}

/// <summary>
/// �X�|�b�g���C�g�R���|�[�l���g�ǉ�
/// </summary>
/// <param name="in_spotL">�ǉ�����X�|�b�g���C�g�R���|�[�l���g</param>
void Renderer::AddSpotLightComponent(SpotLightComponent* in_spotL)
{
	m_spotLights.push_back(in_spotL);
}

/// <summary>
/// �X�|�b�g���C�g�R���|�[�l���g�폜
/// </summary>
/// <param name="in_spotL">�폜����X�|�b�g���C�g�R���|�[�l���g</param>
void Renderer::RemoveSpotLightComponent(SpotLightComponent* in_spotL)
{
	auto iter = std::find(m_spotLights.begin(), m_spotLights.end(), in_spotL);
	m_spotLights.erase(iter);
}

/// <summary>
/// �}�b�vHUD�̒P�̍폜
/// </summary>
void Renderer::RemoveMapHUD()
{
	delete m_mapHUD;
	m_mapHUD = nullptr;
}

// �w�肵���e�N�X�`���̍폜
void Renderer::RemoveTexture(Texture* in_texture)
{
	//auto iter = std::find(m_textures.begin(), m_textures.end(), in_texture);
	//m_textures.erase(iter);

	for (auto iter : m_textures)
	{
		if (iter.second == in_texture)
		{
			m_textures.erase(iter.first);

			break;
		}
	}
}

// �E�B���h�E�̃^�C�g��
void Renderer::SetWindowTitle(const std::string & in_title)
{
	SDL_SetWindowTitle(m_window, in_title.c_str());
}

// �p�[�e�B�N���̒��_�����o�C���h���A�N�e�B�u��
void Renderer::SetWorldSpriteVertex()
{
	m_particleVerts->SetActive();
}


// �X�v���C�g���_�z��̃A�N�e�B�u��
void Renderer::SetActiveSpriteVAO()
{
	m_spriteVerts->SetActive();
}

/// <summary>
/// �e��UBO�̐�������
/// </summary>
void Renderer::CreateUBOs()
{
	// �ϊ��s��UBO
	glGenBuffers(1, &m_uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 128);
	// �J����UBO
	glGenBuffers(1, &m_uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, 16, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_uboCamera, 0, 16);
	// �g���K�[UBO
	glGenBuffers(1, &m_uboTriggers);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboTriggers);
	glBufferData(GL_UNIFORM_BUFFER, 16, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, m_uboTriggers, 0, 16);
	// �f�B���N�V���i�����C�gUBO
	glGenBuffers(1, &m_uboDirLights);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboDirLights);
	glBufferData(GL_UNIFORM_BUFFER, 64, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 3, m_uboDirLights, 0, 64);
}

/// <summary>
/// UBO�ɒl���Z�b�g����
/// </summary>
void Renderer::UpdateUBO()
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);

	// �ϊ��s��UBO
	glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, m_projMat.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, m_viewMat.GetAsFloatPtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �J����UBO
	Vector3 camPos = m_viewMat.GetTranslation();
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, camPos.GetAsFloatPtr());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �g���K�[FBO
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboTriggers);
	int bloom = static_cast<int>(GAME_CONFIG->GetEnableBloom());
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &bloom);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// �f�B���N�V���i�����C�g
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboDirLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 12, m_dirLight->GetDirection().GetAsFloatPtr());
	// ���X�g���C�h������Ȃ�+�ŏ���float���X���[�����̂�
	// ���΍�Ƃ��čŏ���0.0f��Vector2�Ɉ���l��˂����ށ�UBO�̍Ōォ��l�𑗐M
	Vector2 diff1 = Vector2(0.0f, m_dirLight->GetDiffuse().x);
	Vector2 diff2 = Vector2(0.0f, m_dirLight->GetDiffuse().y);
	Vector2 diff3 = Vector2(0.0f, m_dirLight->GetDiffuse().z);
	Vector2 spec1 = Vector2(0.0f, m_dirLight->GetSpecular().x);
	Vector2 spec2 = Vector2(0.0f, m_dirLight->GetSpecular().y);
	Vector2 spec3 = Vector2(0.0f, m_dirLight->GetSpecular().z);
	Vector2 amb1 = Vector2(0.0f, m_dirLight->GetAmbient().x);
	Vector2 amb2 = Vector2(0.0f, m_dirLight->GetAmbient().y);
	Vector2 amb3 = Vector2(0.0f, m_dirLight->GetAmbient().z);
	Vector2 intens = Vector2(0.0f, m_dirLight->GetIntensity());
	glBufferSubData(GL_UNIFORM_BUFFER, 56, 8, intens.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 52, 8, amb3.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 48, 8, amb2.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 44, 8, amb1.GetAsFloatPtr());

	glBufferSubData(GL_UNIFORM_BUFFER, 36, 8, spec3.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 32, 8, spec2.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 28, 8, spec1.GetAsFloatPtr());

	glBufferSubData(GL_UNIFORM_BUFFER, 20, 8, diff3.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 16, 8, diff2.GetAsFloatPtr());
	glBufferSubData(GL_UNIFORM_BUFFER, 12, 8, diff1.GetAsFloatPtr());

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// �e�N�X�`���̎擾
Texture * Renderer::GetTexture(const std::string & in_fileName)
{
	Texture* tex = nullptr;
	auto iter = m_textures.find(in_fileName);     // �����p�X�̃e�N�X�`�������邩������
	// �����e�N�X�`�����o�^���Ă�����
	if (iter != m_textures.end())
	{
		// �����e�N�X�`�����w��
		tex = iter->second;
	}
	else
	{
		// �Ȃ�������e�N�X�`���𐶐����A�z��֓����
		tex = new Texture();
		if (tex->Load(in_fileName))
		{
			m_textures.emplace(in_fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	printf("Load Texture Success : %s \n", in_fileName.c_str());
	return tex;
}

// ���b�V���̎擾
Mesh * Renderer::GetMesh(const std::string & in_fileName)
{
	// �����p�X�̃��b�V������������
	Mesh* mesh = nullptr;
	auto iter = m_meshes.find(in_fileName);
	// ���łɓ������b�V�����o�^���Ă������ꍇ�́A�v�f�̓��(���b�V���N���X)�𕡐�
	if (iter != m_meshes.end())
	{
		mesh = iter->second;
	}
	else
	{

		// �t�@�C���p�X�g���q�𒲂ׁA����ɉ����ă��b�V���𐶐�
		int extNum = in_fileName.find_last_of(".");     // "."�ȍ~�����������邩���ׂ�
		std::string fileExtension = in_fileName.substr(extNum, in_fileName.size() - extNum);    // ".#####"�Ƃ����`�Ŋg���q���擾

		if (fileExtension == ".gpmesh")       // gpmesh�`��
		{
			mesh = new MeshGpmesh();
			if (mesh->Load(in_fileName))
			{
				m_meshes.emplace(in_fileName, mesh);
			}
			else
			{
				std::cerr << "ERROR: Category: " << "MeshComponent: gpmesh�t�@�C���̃��[�h���s" << std::endl;
				delete mesh;
				mesh = nullptr;
			}
		}
		else if (fileExtension == ".obj" || fileExtension == ".OBJ")     // obj�`��
		{
			mesh = new MeshObj();
			if (mesh->Load(in_fileName))
			{
				m_meshes.emplace(in_fileName, mesh);
			}
			else
			{
				std::cerr << "ERROR: Category: " << "MeshComponent: obj�t�@�C���̃��[�h���s" << std::endl;
				delete mesh;
				mesh = nullptr;
			}
		}

	}

	printf("Mesh Load Success : %s\n", in_fileName.c_str());

	return mesh;
}

//-------------------------------------------------------------------------------------+
// �X�P���^�����̎擾
// in  : �X�P���^����� .gpskel�t�@�C����
// out : Skeleton���ւ̃|�C���^
// Desc: gpskel�t�@�C�������A�X�P���^������Ԃ��B�Ȃ��ꍇ�͂��̃t�@�C������
//       �ǂݍ��݂��s���B�ǂݍ��݂��s���Ă��t�@�C�������݂��Ȃ��ꍇ nullptr��Ԃ�
//       ������gpskel�t�@�C�������L�[�Ƃ���X�P���^������map���쐬�����
//-------------------------------------------------------------------------------------+
const Skeleton* Renderer::GetSkeleton(const std::string& in_fileName)
{
	std::string file(in_fileName);
	auto iter = m_skeletons.find(file);
	if (iter != m_skeletons.end())
	{
		return iter->second;
	}
	else
	{
		Skeleton* sk = new Skeleton();
		if (sk->Load(file))
		{
			m_skeletons.emplace(file, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

//-------------------------------------------------------------------------------------------+
// �A�j���[�V�������̎擾
// in  : �A�j���[�V�����f�[�^���i�[����Ă��� .gpanim�t�@�C����
// out : �A�j���[�V�������ւ̃|�C���^
// Desc: gpanim�t�@�C�������A�j���[�V�����f�[�^��Ԃ��B�Ȃ��ꍇ�͂��̃t�@�C������
//       �ǂݍ��݂��s���B�ǂݍ��݂��s���Ă��t�@�C�������݂��Ȃ��ꍇ nullptr��Ԃ�
//       ������gpanim�t�@�C�������L�[�Ƃ���A�j���[�V��������map���쐬�����
//-------------------------------------------------------------------------------------------+
const Animation* Renderer::GetAnimation(const char* in_fileName, bool in_loop)
{
	auto iter = m_animations.find(in_fileName);
	if (iter != m_animations.end())
	{
		return iter->second;
	}
	else
	{
		Animation* anim = new Animation();
		if (anim->Load(in_fileName, in_loop))
		{
			m_animations.emplace(in_fileName, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}

// �p�[�e�B�N���}�l�[�W���[�擾
ParticleManager * Renderer::GetParticleManager() const
{
	return m_particleManager;
}

// �}�b�vHUD�̃Z�b�^�[
void Renderer::SetMapHUD(MiniMapHUD* in_mapHUD)
{
	// �}�b�vHUD����łȂ�������폜
	if (m_mapHUD != nullptr)
	{
		delete m_mapHUD;
	}
	m_mapHUD = in_mapHUD;
}

// �V�F�[�_�[�̃��[�h
bool Renderer::LoadShaders()
{
	/*
	// �X�v���C�g�V�F�[�_�[
	m_spriteShader = new Shader();

	if (!m_spriteShader->Load("Data/Shaders/SpriteShader.vert", "Data/Shaders/SpriteShader.frag"))
	{
		return false;
	}
	m_spriteShader->SetActive();
	// �X�N���[���p�̍s����쐬 (UI��X�v���C�g�͈ȍ~���̍s�����ɕ`��)
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(m_screenWidth, m_screenHeight);
	m_spriteShader->SetMatrixUniform("u_viewProj", viewProj);

	// ���[���h��ԗp�X�v���C�g�V�F�[�_�[
	m_worldSpaceSpriteShader = new Shader();
	if (!m_worldSpaceSpriteShader->Load("Data/Shaders/WorldSpaceSprite.vert", "Data/Shaders/WorldSpaceSprite.frag"))
	{
		return false;
	}


	// ���b�V���V�F�[�_�[
	m_meshShader = new Shader();

	m_view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	m_projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		static_cast<float>(m_screenWidth),
		static_cast<float>(m_screenHeight),
		1.0f, 10000.0f);

	if (!m_meshShader->Load("Data/Shaders/Phong.vert", "Data/Shaders/Phong.frag"))
	{
		return false;
	}

	m_meshShader->SetActive();
	m_meshShader->SetMatrixUniform("u_viewProj", m_view * m_projection);

	// ���b�V���V�F�[�_�[(�@���}�b�v)
	m_meshNormalShader = new Shader();
	if (!m_meshNormalShader->Load("Data/Shaders/NormalMap.vert", "Data/Shaders/NormalMap.frag"))
	{
		return false;
	}
	m_meshNormalShader->SetActive();
	m_meshNormalShader->SetMatrixUniform("u_viewProj", m_view * m_projection);
	m_meshNormalShader->SetVectorUniform("u_lightPos", m_directionalLight.position);
	m_meshNormalShader->SetVectorUniform("u_viewPos", m_cameraPos);
	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_meshNormalShader->SetInt("u_mat.diffuseMap", 0);
	m_meshNormalShader->SetInt("u_mat.specularMap", 1);
	m_meshNormalShader->SetInt("u_mat.normalMap", 2);
	m_meshNormalShader->SetInt("u_mat.depthMap", 3);

	// �X�L���V�F�[�_�[
	m_skinnedShader = new Shader();
	if (!m_skinnedShader->Load("Data/Shaders/Skinned.vert", "Data/Shaders/Phong.frag"))
	{
		return false;
	}
	m_skinnedShader->SetActive();
	m_skinnedShader->SetMatrixUniform("uViewProj", m_view * m_projection);
	// �T���v�����O�p�e�N�X�`���Z�b�g
	m_skinnedShader->SetInt("u_mat.diffuseMap", 0);
	m_skinnedShader->SetInt("u_mat.specularMap", 1);
	m_skinnedShader->SetInt("u_mat.normalMap", 2);
	m_skinnedShader->SetInt("u_mat.depthMap", 3);

	// ���}�b�v�V�F�[�_
	m_environmentMapShader = new Shader();
	if (!m_environmentMapShader->Load("Data/Shaders/EnvironmentMap.vert", "Data/Shaders/EnvironmentMap.frag"))
	{
		return false;
	}

	// �X�J�C�{�b�N�X�p�V�F�[�_�[
	m_skyboxShader = new Shader();
	if (!m_skyboxShader->Load("Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag"))
	{
		return false;
	}

	// �}�b�vHUD���͗p�V�F�[�_�[
	m_mapInputShader = new Shader();
	if (!m_mapInputShader->Load("Data/Shaders/HUD/HUD_MapInput.vert", "Data/Shaders/HUD/HUD_MapInput.frag"))
	{
		return false;
	}

	// �}�b�vHUD�o�͗p�V�F�[�_�[
	m_mapOutputShader = new Shader();
	if (!m_mapOutputShader->Load("Data/Shaders/HUD/HUD_MapOutput.vert", "Data/Shaders/HUD/HUD_MapOutput.frag"))
	{
		return false;
	}
	m_mapOutputShader->SetActive();
	// �X�N���[���p�̍s����쐬 (UI��X�v���C�g�͈ȍ~���̍s�����ɕ`��)
	m_mapOutputShader->SetMatrixUniform("u_viewProj", viewProj);
	*/
	return true;
	
}
