#pragma once
#include "ComputeShader.h"
#include "Texture.h"


namespace mh
{
	class NormalMapEditor : public ComputeShader
	{
	public:
		NormalMapEditor();
		~NormalMapEditor();

		std::shared_ptr<Texture> GetModifiedNormalMap(std::shared_ptr<Texture> _texture, const tCB);

		void SetTarget(std::shared_ptr<Texture> _texture) { mTarget = _texture; }

	protected:
		virtual bool BindData() override;
		virtual void UnBindData() override;
		
		

	private:
		std::shared_ptr<Texture> mTarget;
	};
}
