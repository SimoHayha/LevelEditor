#pragma once

#include <fbxsdk.h>
#include <vector>

#include "FBX_Mesh.h"

namespace id
{
	namespace FBX
	{
		class Animator
		{
		public:
			Animator(void);
			~Animator(void);
			void	setEvaluator(FbxScene* scene);
			void	startTime();
			FbxAnimEvaluator*	getEvaluator();
			FbxTime&			getTime();
			FbxTime				getTotalTime();
			unsigned int&		getCurrentAnim();
			bool				addStack(FbxAnimStack* stack);
			void				startStack(unsigned int i);
			bool				incDt(float dt);
			bool				computeDeformations(id::FBX::Mesh& mesh, FbxVector4* newVertices);
			void				getGlobalMatrix(FbxAMatrix& matrix, FbxNode* node);
			void				getLocalMatrix(FbxAMatrix& matrix, FbxNode* node);
			bool				initAll(FbxScene* scene);
			void				setCurrentAnimation(int animation);
			void				setCurrentAnimationCurrentTime(double d);
			bool				isLoop() const;
			void				setLoop(bool);
			void				next();
			void				setSpeed(float);
			float				getSpeed() const;

			size_t							getNumAnim() const;
			FbxArray<FbxString *> const &	getAnimNames() const;
		private:
			std::vector<FbxAnimStack*>		animStack;
			FbxArray<FbxString *>			animStackName;
			FbxAnimEvaluator*				evaluator;
			FbxTime							time;
			unsigned int					currentAnim;
			size_t							numAnim;
			bool							loop;
			float							speed;
		};
	}
}
