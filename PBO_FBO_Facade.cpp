#include "PBO_FBO_Facade.h"

 PBO_FBO_Facade:: PBO_FBO_Facade(FBOManager  &FBOMgr,PBOReceiver & PBORcr):
	m_FBOMgr(FBOMgr),
	m_PBORcr(PBORcr)
 {
}
void PBO_FBO_Facade::DrawAndGet()
{
	m_FBOMgr.InToFrameBuffer();
	m_FBOMgr.DrawFrame();
	m_PBORcr.getData();
	m_FBOMgr.OutOfFrameBuffer();
}

