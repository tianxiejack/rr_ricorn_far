/*
 * multiLayerButtonGroup.h
 *
 *  Created on: 2018年8月1日
 *      Author: fsmdn121
 */

#ifndef MULTILAYERBUTTONGROUP_H_
#define MULTILAYERBUTTONGROUP_H_
#include "set_button.h"

class multiLayerButtonGroup:public InterfaceButtonGroup
{
public:
	multiLayerButtonGroup( InterfaceRenderBehavior *p,int groupCount = 1);
	~multiLayerButtonGroup();
	void Group_Draw();
	void Update_State();
	void init_button_group(GLShaderManager *shaderManager,GLMatrixStack * modelViewMatrix,
			GLMatrixStack * projectionMtrx, GLFrustum *     pViewFrustrm);
	void SetEnableDraw(bool enable);
	bool GetEnableDraw(){return enable_draw;};
	void SetcurrentActiveBGIndex(int idx){currentActiveBGIndex=idx;};
	void SetSubmenuKeycode(int keycode){m_submenuKeycode = keycode;};
private:
	std::vector<ButtonGroup*> m_layeredButtonGroupsVect;
	int currentActiveBGIndex;
	int m_submenuKeycode;
	ButtonGroup * pbuttongroups;
	bool enable_draw;
	GLMatrixStack * p_modelViewMatrix;
	GLMatrixStack * p_projectionMatrix;
	GLFrustum *     pViewFrustrm;
    InterfaceRenderBehavior* p_Host;
    int counter;
    static const int AUTO_HIDE_COUNT=250;
};






#endif /* MULTILAYERBUTTONGROUP_H_ */
