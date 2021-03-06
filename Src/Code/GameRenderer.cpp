/// \file gamerenderer.cpp
/// \brief Direct3D rendering tasks for the game.
/// DirectX stuff that won't change much is hidden away in this file
/// so you won't have to keep looking at it.

#include <algorithm>

#include "gamerenderer.h"
#include "defines.h" 
#include "abort.h"
#include "imagefilenamelist.h"
#include "debug.h"
#include "sprite.h"
#include "object.h"
#include "GameController.h"

extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern bool stage1Complete;
extern bool shapesPuzzleSolved;
extern bool cardPuzzleSolved;
extern bool wirePuzzleSolved;
extern bool gameWon;
extern bool shapeOnScreen;
extern BOOL g_bWireFrame;
extern bool menu_Screen;
extern bool fail;
extern C3DSprite* g_pBriefcaseSprite;
extern C3DSprite* g_pArrows;
extern C3DSprite* g_mainMenu;
extern C3DSprite* g_pLedLitSprite;
extern C3DSprite* g_pLedLitSpriteCard;
extern C3DSprite* g_pLedLitSpriteWire;
extern C3DSprite* g_numberSprite0;
extern C3DSprite* g_numberSprite1;
extern C3DSprite* g_numberSprite2;
extern C3DSprite* g_numberSprite3;
extern C3DSprite* g_creditCardClue;
extern C3DSprite* g_numberCardSprite0;
extern C3DSprite* g_numberCardSprite1;
extern C3DSprite* g_numberCardSprite2;
extern C3DSprite* g_numberCardSprite3;
extern C3DSprite* g_numberBarcodeSprite0;
extern C3DSprite* g_numberBarcodeSprite1;
extern C3DSprite* g_numberBarcodeSprite2;
extern C3DSprite* g_numberBarcodeSprite3;
extern C3DSprite* g_pArrows0;
extern C3DSprite* g_pArrows1;
extern C3DSprite* g_pArrows2;
extern C3DSprite* g_pArrows3;
extern C3DSprite* g_pTimerNumber0;
extern C3DSprite* g_pTimerNumber1;
extern C3DSprite* g_pTimerNumber2;
extern C3DSprite* g_pTimerNumber3;
extern C3DSprite* g_pTimerNumber4;
extern C3DSprite* g_pTimerNumber5;
extern C3DSprite* g_pTimerNumber6;
extern C3DSprite* g_pTimerNumber7;
extern C3DSprite* g_pTimerNumber8;
extern C3DSprite* g_pTimerNumber9;
extern C3DSprite* g_cardDigitkey1;
extern C3DSprite* g_cardDigitkey2;
extern C3DSprite* g_cardDigitkey3;
extern C3DSprite* g_cardDigitkey4;
extern C3DSprite* g_shapeClueSprite1;
extern C3DSprite* g_shapeClueSprite2;
extern C3DSprite* g_shapeClueSprite3;
extern C3DSprite* g_shapeClueSprite4;
extern C3DSprite* g_wireSprite1;
extern C3DSprite* g_wireSprite2;
extern C3DSprite* g_wireSprite3;
extern C3DSprite* g_wireSprite4;
extern C3DSprite* g_wireSpriteStage1;
extern C3DSprite* g_wireSpriteStage2;
extern C3DSprite* g_wireSpriteStage3;
extern C3DSprite* g_wireSpriteStage4;
extern C3DSprite* g_pStrike1;
extern C3DSprite* g_pStrike2;
extern C3DSprite* g_pStrike3;
extern C3DSprite* g_StageTwo;
extern C3DSprite* g_failScreen;
extern C3DSprite* g_pWinScreen;
extern C3DSprite* g_pMinuteRightSprite;
extern C3DSprite* g_pSecondsRightSprite;
extern C3DSprite* g_pSecondsLefttSprite;
extern C3DSprite* g_pBarcodeCaseSprite;
extern C3DSprite* g_pTopCluesBannerSprite;
extern CImageFileNameList g_cImageFileName;
void briefcaseRotation(int x, int y);
void DrawBriefcase();	
void drawMenuScreen();
void DrawStrikes();
void DrawTimer();
void winGameFunc();
void loadShapeScreen(int);
extern void drawShapeScreen();
extern void drawCardScreen();
extern void drawWireScreen();
extern CGameObject* g_pTopClues; 
extern CGameObject* g_pBarcode;
extern CGameObject* g_pBarcodeCase;
extern GameController mainController;
extern CTimer g_cTimer; ///< The game timer

CGameRenderer::CGameRenderer(): m_bCameraDefaultMode(TRUE){
} //constructor


/// Initialize the vertex and constant buffers for the background, that is, the
/// ground and the sky.

void CGameRenderer::InitBackground(){
  HRESULT hr;

  //load vertex buffer
  float w = 2.0f*g_nScreenWidth;
  float h = 2.0f*g_nScreenHeight;
  
  //vertex information, first triangle in clockwise order
  BILLBOARDVERTEX pVertexBufferData[6]; 
  pVertexBufferData[0].p = Vector3(w, 0, 0);
  pVertexBufferData[0].tu = 1.0f; pVertexBufferData[0].tv = 0.0f;

  pVertexBufferData[1].p = Vector3(0, 0, 0);
  pVertexBufferData[1].tu = 0.0f; pVertexBufferData[1].tv = 0.0f;

  pVertexBufferData[2].p = Vector3(w, 0, 1500);
  pVertexBufferData[2].tu = 1.0f; pVertexBufferData[2].tv = 1.0f;

  pVertexBufferData[3].p = Vector3(0, 0, 1500);
  pVertexBufferData[3].tu = 0.0f; pVertexBufferData[3].tv = 1.0f;

  pVertexBufferData[4].p = Vector3(w, h, 1500);
  pVertexBufferData[4].tu = 1.0f; pVertexBufferData[4].tv = 0.0f;

  pVertexBufferData[5].p = Vector3(0, h, 1500);
  pVertexBufferData[5].tu = 0.0f; pVertexBufferData[5].tv = 0.0f;
  
  //create vertex buffer for background
  m_pShader = new CShader(2);
    
  m_pShader->AddInputElementDesc(0, DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
  m_pShader->AddInputElementDesc(12, DXGI_FORMAT_R32G32_FLOAT,  "TEXCOORD");
  m_pShader->VSCreateAndCompile(L"VertexShader.hlsl", "main");
  m_pShader->PSCreateAndCompile(L"PixelShader.hlsl", "main");
    
  // Create constant buffers.
  D3D11_BUFFER_DESC constantBufferDesc = { 0 };
  constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
  constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  constantBufferDesc.CPUAccessFlags = 0;
  constantBufferDesc.MiscFlags = 0;
  constantBufferDesc.StructureByteStride = 0;
    
  m_pDev2->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffer);
    
  D3D11_BUFFER_DESC VertexBufferDesc;
  VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  VertexBufferDesc.ByteWidth = sizeof(BILLBOARDVERTEX)* 6;
  VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  VertexBufferDesc.CPUAccessFlags = 0;
  VertexBufferDesc.MiscFlags = 0;
  VertexBufferDesc.StructureByteStride = 0;
    
  D3D11_SUBRESOURCE_DATA subresourceData;
  subresourceData.pSysMem = pVertexBufferData;
  subresourceData.SysMemPitch = 0;
  subresourceData.SysMemSlicePitch = 0;
    
  hr = m_pDev2->CreateBuffer(&VertexBufferDesc, &subresourceData, &m_pBackgroundVB);
} //InitBackground

/// Draw the game background.

void CGameRenderer::DrawBackground(){
  UINT nVertexBufferOffset = 0;
  
  UINT nVertexBufferStride = sizeof(BILLBOARDVERTEX);
  m_pDC2->IASetVertexBuffers(0, 1, &m_pBackgroundVB, &nVertexBufferStride, &nVertexBufferOffset);
  m_pDC2->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  m_pShader->SetShaders();

  //draw floor
  if(g_bWireFrame)
    m_pDC2->PSSetShaderResources(0, 1, &m_pWireframeTexture); //set wireframe texture
  else
    m_pDC2->PSSetShaderResources(0, 1, &m_pFloorTexture); //set floor texture
  
  SetWorldMatrix();
  
  ConstantBuffer constantBufferData; ///< Constant buffer data for shader.

  constantBufferData.wvp = CalculateWorldViewProjectionMatrix();
  m_pDC2->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &constantBufferData, 0, 0);
  m_pDC2->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
  m_pDC2->Draw(4, 0);

  //draw backdrop
  if(!g_bWireFrame)
    m_pDC2->PSSetShaderResources(0, 1, &m_pWallTexture);

  constantBufferData.wvp = CalculateWorldViewProjectionMatrix();
  m_pDC2->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &constantBufferData, 0, 0);
  m_pDC2->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
  m_pDC2->Draw(4, 2);
} //DrawBackground
 
/// Load the background and sprite textures.

void CGameRenderer::LoadTextures(){ 
  LoadTexture(m_pWallTexture, g_cImageFileName[0]);
  LoadTexture(m_pFloorTexture, g_cImageFileName[1]);
  //LoadTexture(m_pWireframeTexture, g_cImageFileName[2]); //black for wireframe
} //LoadTextures

/// All textures used in the game are released - the release function is kind
/// of like a destructor for DirectX entities, which are COM objects.

void CGameRenderer::Release(){ 
  g_pBriefcaseSprite->Release();
  g_pArrows->Release();
  g_mainMenu->Release();
  g_pLedLitSprite->Release();
  g_pLedLitSpriteCard->Release();
  g_pLedLitSpriteWire->Release();
  g_numberSprite0->Release();
  g_numberSprite1->Release();
  g_numberSprite2->Release();
  g_numberSprite3->Release();
  g_creditCardClue->Release();
  g_numberCardSprite0->Release();
  g_numberCardSprite1->Release();
  g_numberCardSprite2->Release();
  g_numberCardSprite3->Release();
  g_numberBarcodeSprite0->Release();
  g_numberBarcodeSprite1->Release();
  g_numberBarcodeSprite2->Release();
  g_numberBarcodeSprite3->Release();
  g_pArrows0->Release();
  g_pArrows1->Release();
  g_pArrows2->Release();
  g_pArrows3->Release();
  g_pTimerNumber0->Release();
  g_pTimerNumber1->Release();
  g_pTimerNumber2->Release();
  g_pTimerNumber3->Release();
  g_pTimerNumber4->Release();
  g_pTimerNumber5->Release();
  g_pTimerNumber6->Release();
  g_pTimerNumber7->Release();
  g_pTimerNumber8->Release();
  g_pTimerNumber9->Release();
  g_wireSprite1->Release();
  g_wireSprite2->Release();
  g_wireSprite3->Release();
  g_wireSprite4->Release();
  g_wireSpriteStage1->Release();
  g_wireSpriteStage2->Release();
  g_wireSpriteStage3->Release();
  g_wireSpriteStage4->Release();
  g_pStrike1->Release();
  g_pStrike2->Release();
  g_pStrike3->Release();
  g_pBarcodeCaseSprite->Release();
  g_pTopCluesBannerSprite->Release();

  SAFE_RELEASE(m_pWallTexture);
  SAFE_RELEASE(m_pFloorTexture);
  SAFE_RELEASE(m_pWireframeTexture);
  SAFE_RELEASE(m_pBackgroundVB);

  SAFE_DELETE(m_pShader);
  
  CRenderer::Release();
} //Release

/// Move all objects, then draw them.
/// \return TRUE if it succeeded

void CGameRenderer::ComposeFrame(){ 

  //prepare to draw
  m_pDC2->OMSetRenderTargets(1, &m_pRTV, m_pDSV);
  float clearColor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
  m_pDC2->ClearRenderTargetView(m_pRTV, clearColor);
  m_pDC2->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

  //draw
  if(!menu_Screen) {
	  DrawBackground(); //draw background 
	  DrawBriefcase();
	  g_pTopClues->draw(); //draw plane
	  g_pBarcode->draw(); // draw barcode
	  g_pBarcodeCase->draw();//Draw barcode on case
	  DrawStrikes(); // Draw Bomb Strikes
	  if (!fail)
		  DrawTimer();
	  if(stage1Complete) {
		  if(drawShapeScreen)
			  drawShapeScreen();
			  drawCardScreen();
			  drawWireScreen();
	  }
  }
  else {
	  drawMenuScreen();
  }
  if (shapesPuzzleSolved == true && cardPuzzleSolved == true && wirePuzzleSolved == true) {
	  winGameFunc();
	  gameWon = true;
  }
} //ComposeFrame
 
/// Compose a frame of animation and present it to the video card.

void CGameRenderer::ProcessFrame() {
	ComposeFrame(); //compose a frame of animation
	m_pSwapChain2->Present(1, 0); //present it
} //ProcessFrame

/// Toggle between eagle-eye camera (camera pulled back far enough to see
/// backdrop) and the normal game camera.

void CGameRenderer::FlipCameraMode(){

} //FlipCameraMode
