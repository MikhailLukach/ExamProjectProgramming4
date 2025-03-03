#include "CacheTestComponent.h"

//make a render non-const loop specifically for ImGui
//fix layout and how sampling is done, not literal number of sample but number of iterations
struct Transform
{
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1 };
};

class GameObject3D
{
public:
    Transform transform;
    int ID = 0;
};

class GameObject3DAlt
{
public:
    Transform* transform;
    int ID = 0;
};

dae::CacheTestComponent::CacheTestComponent()
{
    for (int stepSize = 1; stepSize <= 1024; stepSize *= 2)
    {
        m_StepSizes.push_back(static_cast<float>(stepSize));
    }
    RunPerformanceTest();
}

void dae::CacheTestComponent::Update(float deltaTime)
{
    m_currentDeltaTime = deltaTime;
    if (m_NeedUpdate)
    {
        RunPerformanceTest();
    }
}

void dae::CacheTestComponent::Render() const
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Performance Test");

    ImGui::InputInt("Sample Size", const_cast<int*>(&m_SampleSize));

    if (ImGui::Button("+"))
    {
        const_cast<CacheTestComponent*>(this)->m_SampleSize *= 2;
    }
    ImGui::SameLine(0.f, 1.f);

    if (ImGui::Button("-") && m_SampleSize/2 >= 0)
    {
        const_cast<CacheTestComponent*>(this)->m_SampleSize /= 2;
    }

    if (ImGui::Button("Generate Line Graph"))
    {
        const_cast<CacheTestComponent*>(this)->m_ShowGraph = !m_ShowGraph;
        const_cast<CacheTestComponent*>(this)->m_NeedUpdate = true;
    }

    if (m_ShowGraph && !m_Times.empty())
    {
        ImGui::PlotConfig conf;
        conf.values.xs = nullptr;
        conf.values.ys = m_Times.data();
        conf.values.count = static_cast<int>(m_Times.size());
        conf.scale.min = 0.0f;
        conf.scale.max = *std::max_element(m_Times.begin(), m_Times.end()) > 0 ?
            *std::max_element(m_Times.begin(), m_Times.end()) : 1.0f;
        conf.tooltip.show = true;
        conf.tooltip.format = "Step: %d, Time: %.2f";
        conf.grid_x.show = true;
        conf.grid_x.size = 128;
        conf.grid_y.show = true;
        conf.grid_y.size = 0.5f;
        conf.frame_size = ImVec2(400, 150);
        conf.line_thickness = 2.0f;

        ImGui::Plot("Performance Graph", conf);
    }


    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(500, 0), ImGuiCond_FirstUseEver);

    ImGui::Begin("Performance Test 2");

    ImGui::InputInt("Sample Size", const_cast<int*>(&m_SampleSize2));

    if (ImGui::Button("+"))
    {
        const_cast<CacheTestComponent*>(this)->m_SampleSize2 *= 2;
    }
    ImGui::SameLine(0.f, 1.f);

    if (ImGui::Button("-") && m_SampleSize2 / 2 >= 0)
    {
        const_cast<CacheTestComponent*>(this)->m_SampleSize2 /= 2;
    }

    if (ImGui::Button("Generate GameObject Graph"))
    {
        const_cast<CacheTestComponent*>(this)->m_ShowGameObjectGraph = true;
        const_cast<CacheTestComponent*>(this)->m_ShowGameObjectAltGraph = false;
        const_cast<CacheTestComponent*>(this)->m_ShowCombinedGraph = false;
        const_cast<CacheTestComponent*>(this)->m_NeedUpdate = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Generate GameObjectAlt & Combined Graph"))
    {
        const_cast<CacheTestComponent*>(this)->m_ShowGameObjectGraph = false;
        const_cast<CacheTestComponent*>(this)->m_ShowGameObjectAltGraph = true;
        const_cast<CacheTestComponent*>(this)->m_ShowCombinedGraph = true;
        const_cast<CacheTestComponent*>(this)->m_NeedUpdate = true;
    }

    if (m_ShowGameObjectGraph && !m_TimesGB.empty())
    {
        ImGui::PlotConfig conf;
        conf.values.ys = m_TimesGB.data();
        conf.values.count = static_cast<int>(m_TimesGB.size());
        conf.scale.min = 0;
        conf.scale.max = *std::max_element(m_TimesGB.begin(), m_TimesGB.end());
        conf.tooltip.show = true;
        conf.tooltip.format = "Step=%d, Time=%.2f";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(400, 150);
        conf.line_thickness = 2.0f;

        ImGui::Plot("GameObject Performance Graph", conf);
    }

    if (m_ShowGameObjectAltGraph && !m_TimesGBAlt.empty())
    {
        ImGui::PlotConfig conf;
        conf.values.ys = m_TimesGBAlt.data();
        conf.values.count = static_cast<int>(m_TimesGBAlt.size());
        conf.scale.min = 0;
        conf.scale.max = *std::max_element(m_TimesGBAlt.begin(), m_TimesGBAlt.end());
        conf.tooltip.show = true;
        conf.tooltip.format = "Step=%d, Time=%.2f";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(400, 150);
        conf.line_thickness = 2.0f;

        ImGui::Plot("GameObjectAlt Performance Graph", conf);
    }

    if (m_ShowCombinedGraph && !m_TimesGB.empty() && !m_TimesGBAlt.empty())
    {
        static const float* y_data[] = { m_TimesGB.data(), m_TimesGBAlt.data() };
        static ImU32 colors[2] = { IM_COL32(0, 255, 0, 255), IM_COL32(255, 0, 0, 255) };

        ImGui::PlotConfig conf;
        conf.values.ys_list = y_data;
        conf.values.ys_count = 2;
        conf.values.count = static_cast<int>(std::min(m_TimesGB.size(), m_TimesGBAlt.size()));
        conf.values.colors = colors;
        conf.scale.min = 0;
        conf.scale.max = std::max(*std::max_element(m_TimesGB.begin(), m_TimesGB.end()),
            *std::max_element(m_TimesGBAlt.begin(), m_TimesGBAlt.end()));
        conf.tooltip.show = true;
        conf.tooltip.format = "Step=%d, Time=%.2f";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(400, 150);
        conf.line_thickness = 2.0f;

        ImGui::Plot("Combined Performance Graph", conf);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void dae::CacheTestComponent::RunPerformanceTest()
{
    std::vector<long long> times;
    std::vector<long long> timesGB;
    std::vector<long long> timesGBAlt;

    int* arrInt = new int[m_SampleSize] {};
    GameObject3D* arrGameObjects = new GameObject3D[m_SampleSize2];
    GameObject3DAlt* arrGameObjectsAlt = new GameObject3DAlt[m_SampleSize2];

    std::cout << "Running performance test with sample size: " << m_SampleSize << std::endl;

    std::cout << "Running performance test with sample2 size: " << m_SampleSize2 << std::endl;

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < m_SampleSize; i += stepsize)
        {
            arrInt[i] *= 2;
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        times.push_back(elapsedTime);

        std::cout << "StepSize " << stepsize << " - Time: " << elapsedTime << " (int)" << std::endl;
    }

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < m_SampleSize2; i += stepsize)
        {
            arrGameObjects[i].ID += 2;
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        timesGB.push_back(elapsedTime);

        std::cout << "StepSize " << stepsize << " - Time: " << elapsedTime << " (GameObject3D)" << std::endl;
    }

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < m_SampleSize2; i += stepsize)
        {
            arrGameObjectsAlt[i].ID += 2;
        }
        const auto end = std::chrono::high_resolution_clock::now();
        const auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        timesGBAlt.push_back(elapsedTime);

        std::cout << "StepSize " << stepsize << " - Time: " << elapsedTime << " (GameObject3DAlt)" << std::endl;
    }

    delete[] arrInt;
    delete[] arrGameObjects;
    delete[] arrGameObjectsAlt;

    m_Times.clear();
    m_TimesGB.clear();
    m_TimesGBAlt.clear();

    for (size_t i = 0; i < timesGB.size(); ++i)
    {
        m_TimesGB.push_back(static_cast<float>(timesGB[i]));
        m_TimesGBAlt.push_back(static_cast<float>(timesGBAlt[i]));
    }
    for (size_t i = 0; i < times.size(); ++i)
    {
        m_Times.push_back(static_cast<float>(times[i]));
    }

    std::cout << "Performance test complete! Data stored." << std::endl;

    m_NeedUpdate = false;
}
