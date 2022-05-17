#include "../PainterEngine/kernel/PX_Kernel.h"

#define LCD_FB_START_ADDRESS       ((uint32_t)0xC0000000)			//LTDC Select Layer 0


px_byte cache[16*1024*1024] __attribute__((at(LCD_FB_START_ADDRESS+1024*1024*8))); 

//ABGR8888>>ARGB8888

void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t ColorMode)
{    
  /* 配置DMA2D模式,颜色模式和输出偏移 */
  Dma2d_Handler.Init.Mode         = DMA2D_M2M_PFC;
  Dma2d_Handler.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  Dma2d_Handler.Init.OutputOffset = 0;     
	Dma2d_Handler.Init.RedBlueSwap = DMA2D_RB_SWAP;
  
  /* Foreground Configuration */
  Dma2d_Handler.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  Dma2d_Handler.LayerCfg[1].InputAlpha = 0xFF;
  Dma2d_Handler.LayerCfg[1].InputColorMode = ColorMode;
  Dma2d_Handler.LayerCfg[1].InputOffset = 0;
  
  Dma2d_Handler.Instance = DMA2D; 
  
  /* DMA2D 初始化 */
  if(HAL_DMA2D_Init(&Dma2d_Handler) == HAL_OK) 
  {
    if(HAL_DMA2D_ConfigLayer(&Dma2d_Handler, 1) == HAL_OK) 
    {
      if (HAL_DMA2D_Start(&Dma2d_Handler, (uint32_t)pSrc, (uint32_t)pDst, 800, 480) == HAL_OK)
      {
        /* DMA轮询传输*/  
        HAL_DMA2D_PollForTransfer(&Dma2d_Handler, 1000);
      }
    }
  } 
}



void px_demo(void){

	px_surface rendertarget;
	px_memorypool mp;
	mp = MP_Create(fb,sizeof(fb));
	PX_SurfaceCreate(&mp,800,480,&rendertarget);
	PX_SurfaceClear(&rendertarget,0,0,rendertarget.width-1,rendertarget.height-1,PX_COLOR(255,255,000,000)); //ABGR
	//	LCD_LayerInit(1, (uint32_t)rendertarget.surfaceBuffer,ARGB8888);
	//	LCD_SelectLayer(1);
	PX_GeoDrawLine(&rendertarget, 100, 100, 600, 100, 2, PX_COLOR(255, 255, 0, 0));
	PX_GeoDrawLine(&rendertarget, 100, 200, 600, 200, 6, PX_COLOR(255, 0, 255, 0));
	PX_GeoDrawLine(&rendertarget, 100, 300, 600, 300, 12, PX_COLOR(255, 0, 0, 255));

	LL_ConvertLineToARGB8888(rendertarget.surfaceBuffer,LCD_FB_START_ADDRESS,ARGB8888);
	for(;;){
		
		
		
	}

}