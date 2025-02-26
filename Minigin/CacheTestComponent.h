#pragma once
#include "Component.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <vector>
#include <chrono>
#include <iostream>

namespace dae
{
	class CacheTestComponent : public Component
	{
	public:
		CacheTestComponent();
		void Update(float deltaTime) override;
		void Render() const override;
	private:
		int m_SampleSize = 10;
		int m_SampleSize2 = 10;
		std::vector<float> m_StepSizes;
		std::vector<float> m_Times;
		std::vector<float> m_TimesGB;
		std::vector<float> m_TimesGBAlt;
		bool m_NeedUpdate = true;
		bool m_ShowGraph = false;
		int m_SelectedGraph = 0;
		float m_currentDeltaTime;

		bool m_ShowGameObjectGraph = false;
		bool m_ShowGameObjectAltGraph = false;
		bool m_ShowCombinedGraph = false;


		void RunPerformanceTest();
		void IncreaseSampleSize();
		void DecreaseSampleSize();
	};
}

