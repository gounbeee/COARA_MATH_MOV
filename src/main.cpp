#include <iostream>



#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>



#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif



// // FFmpeg library
// extern "C" {
// #include "libavutil/imgutils.h"
// #include "libavcodec/avcodec.h"
// #include "libavformat/avformat.h"
// }
// // #pragma comment(lib, "avutil.lib")
// // #pragma comment(lib, "avcodec.lib")
// // #pragma comment(lib, "avformat.lib")
// void on_frame_decoded(AVFrame* frame) {
// 	printf("Frame decoded PTS: %jd\n", frame->pts);
// }



#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"




//The music that will be played
Mix_Music *gMusic = NULL;

// //The sound effects that will be used
// Mix_Chunk *gScratch = NULL;
// Mix_Chunk *gHigh = NULL;
// Mix_Chunk *gMedium = NULL;
// Mix_Chunk *gLow = NULL;





bool loadMedia()
{
	//Loading success flag
	bool success = true;

	// //Load prompt texture
	// if( !gPromptTexture.loadFromFile( "21_sound_effects_and_music/prompt.png" ) )
	// {
	// 	printf( "Failed to load prompt texture!\n" );
	// 	success = false;
	// }

	//Load music
	gMusic = Mix_LoadMUS( "resources/sample_1.mp3" );
	if( gMusic == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	
	// //Load sound effects
	// gScratch = Mix_LoadWAV( "21_sound_effects_and_music/scratch.wav" );
	// if( gScratch == NULL )
	// {
	// 	printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	// 	success = false;
	// }
	
	// gHigh = Mix_LoadWAV( "21_sound_effects_and_music/high.wav" );
	// if( gHigh == NULL )
	// {
	// 	printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	// 	success = false;
	// }

	// gMedium = Mix_LoadWAV( "21_sound_effects_and_music/medium.wav" );
	// if( gMedium == NULL )
	// {
	// 	printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	// 	success = false;
	// }

	// gLow = Mix_LoadWAV( "21_sound_effects_and_music/low.wav" );
	// if( gLow == NULL )
	// {
	// 	printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	// 	success = false;
	// }

	return success;
}






// Main code
int main(int, char**)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to the latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;







    // // ------------------------------------------------------------------------
    // // ------------------------------------------------------------------------



    // // INITIALIZE FFMPEG ????
    // // *****  YOU DO NOT NEED THIS FROM VERSION 4.0 !!!!
    // // https://github.com/leandromoreira/ffmpeg-libav-tutorial/issues/29

    // //av_register_all();
	// //avcodec_register_all();
	// //avfilter_register_all();



	// const char* input_path = "resources/sample-15s.mp4";
	// AVFormatContext* format_context = nullptr;

	// if (avformat_open_input(&format_context, input_path, nullptr, nullptr) != 0) {
	// 	printf("avformat_open_input failed\n");
	// }

	// if (avformat_find_stream_info(format_context, nullptr) < 0) {
	// 	printf("avformat_find_stream_info failed\n");
	// }

	// AVStream* video_stream = nullptr;
	// for (int i = 0; i < (int)format_context->nb_streams; ++i) {

	// 	if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
	// 	  	video_stream = format_context->streams[i];
	// 	  	break;
	// 	}

	// }


	// if (video_stream == nullptr) {
	// 	printf("No video stream ...\n");
	// }

	// AVCodec* codec = avcodec_find_decoder(video_stream->codecpar->codec_id);

	// if (codec == nullptr) {
	// 	printf("No supported decoder ...\n");
	// }

	// AVCodecContext* codec_context = avcodec_alloc_context3(codec);

	// if (codec_context == nullptr) {
	// 	printf("avcodec_alloc_context3 failed\n");
	// }

	// if (avcodec_parameters_to_context(codec_context, video_stream->codecpar) < 0) {
	// 	printf("avcodec_parameters_to_context failed\n");
	// }

	// if (avcodec_open2(codec_context, codec, nullptr) != 0) {
	// 	printf("avcodec_open2 failed\n");
	// }


	// AVFrame* frame = av_frame_alloc();
	// AVPacket packet = AVPacket();

	// while (av_read_frame(format_context, &packet) == 0) {



	// 	if (packet.stream_index == video_stream->index) {
	// 	  	if (avcodec_send_packet(codec_context, &packet) != 0) {
	// 	    	printf("avcodec_send_packet failed\n");
	// 	  	}

	// 	  	while (avcodec_receive_frame(codec_context, frame) == 0) {
	// 	    	on_frame_decoded(frame);
	// 	  	}
	// 	}

	// 	av_packet_unref(&packet);


	// }


	// // flush decoder
	// if (avcodec_send_packet(codec_context, nullptr) != 0) {
	// 	printf("avcodec_send_packet failed");
	// }

	// while (avcodec_receive_frame(codec_context, frame) == 0) {
	// 	on_frame_decoded(frame);
	// }







    // // ------------------------------------------------------------------------
    // // ------------------------------------------------------------------------





	cv::VideoCapture cap;
	cap.open("resources/sample_1.mov");
	cv::Mat frame;





    // // ------------------------------------------------------------------------
    // // ------------------------------------------------------------------------

	//Initialization flag
	bool mixer_success = true;

	 //Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		mixer_success = false;
	}


	//Load media
	if( !loadMedia() )
	{
		printf( "Failed to load media!\n" );
	}






    while (!done)
    {




		// < PLAYING SOUND USING OPENCV ! > 
		// https://shizenkarasuzon.hatenablog.com/entry/2020/03/21/000437#%E6%94%B9%E8%89%AF%E9%9F%B3%E5%A3%B0%E5%86%8D%E7%94%9F


		cap.read(frame); 								//1フレーム分取り出してimgに保持させる
		if (frame.empty()) { 							//読み込んだ画像がempty、つまり最終フレームに達したとき
			cap.set(cv::CAP_PROP_POS_FRAMES, 0);  		//また最初から再生し直す
			cap.read(frame);
		}


    	cv::imshow( "Example 2-3", frame );

	    if( (char)cv::waitKey(33) >= 0 ) break;








		//If there is no music playing
		if( Mix_PlayingMusic() == 0 )
		{
			//Play the music
			Mix_PlayMusic( gMusic, -1 );
		}
		//If music is being played
		else
		{
			// //If the music is paused
			// if( Mix_PausedMusic() == 1 )
			// {
			// 	//Resume the music
			// 	Mix_ResumeMusic();
			// }
			// //If the music is playing
			// else
			// {
			// 	//Pause the music
			// 	Mix_PauseMusic();
			// }
		}








        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }



        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();



        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();




        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);




        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        

		// CAPTURE FRAME
	    //cap >> frame;






        SDL_GL_SwapWindow(window);



    }







	// av_frame_free(&frame);
	// avcodec_free_context(&codec_context);
	// avformat_close_input(&format_context);


	//Stop the music
	Mix_HaltMusic();




    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;


}







