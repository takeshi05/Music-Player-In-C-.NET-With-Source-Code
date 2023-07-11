// Created by Antonis Ntit  
// spanomarias68@gmail.com
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <filesystem>
#include "libZPlayNET.h"

namespace SoundPlayer
{

	using namespace System;
	using namespace System::IO;
	using namespace System::Globalization;
	using namespace System::Resources;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace libZPlay;
	namespace files = std::experimental::filesystem;

	/// <summary>
	/// Summary for PlayerForm
	/// </summary>
	public ref class PlayerForm : public System::Windows::Forms::Form
	{
	public:
		ZPlay ^ player;
		PlayerForm(void)
		{
			InitializeComponent();
			player = gcnew  ZPlay();
			print("Library Version " + player->GetVersion());
			resReader = gcnew ResXResourceReader("./PlayerForm.resx");
			resWriter = gcnew ResXResourceWriter("./PlayerForm.resx");
			resourcArr = gcnew array<String^>(17)
			{
				"b0V", "b1V", "b2V", "b3V", "b4V",
					"b5V", "b6V", "b7V", "b8V",
					"soundsDirPath", "enableEqu", "masterVolume",
					"leftVolume", "rightVolume", "pitchValue", "tempoValue", "rateValue"
			};
			reset();
			clearLabelsInfo();
			equalizerSet();
			resourceRead();
			pathLbl->Text = soundsDirPath;
			updateTimer->Stop();
			loadTracks();
			vuMeterOff();
		}


	private:
		array<String^> ^resourcArr;
		String ^ soundsDirPath = "";
		String^ trackName = "";
		String^ playingTrack = "";
		String^ H;
		String^	M;
		String^ S;
		int trackListIndex = 0;
		int visCounter = 0;
		int leftChannelVu;
		int rightChannelVu;
		int masterVolume;
		int leftVolume, rightVolume;
		const int freqBandsCount = 9;
		ResXResourceReader^ resReader;
		ResXResourceWriter^ resWriter;
		TStreamTime timeInfo;
		TStreamInfo streamInfo;
		TStreamStatus statusInfo;
		TStreamTime curPositionTrack;
		TStreamTime totalLenghtTrack;


#pragma region Windows Form Components

		System::Windows::Forms::ListBox^  trackList;
		System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog;
		System::Windows::Forms::Button^  dirBtn;
		System::ComponentModel::IContainer^  components;
		System::Windows::Forms::Label^  pathLbl;
		System::Windows::Forms::Panel^  playBtnPanel;
		System::Windows::Forms::Button^  playStopBtn;
		System::Windows::Forms::Button^  pauseBtn;
		System::Windows::Forms::Label^  label4;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Label^  label6;
		System::Windows::Forms::Label^  label5;
		System::Windows::Forms::Label^  trackLbl;
		System::Windows::Forms::Label^  yearLbl;
		System::Windows::Forms::Label^  artistLbl;
		System::Windows::Forms::Label^  titleLbl;
		System::Windows::Forms::Label^  totalTimeLbl;
		System::Windows::Forms::Timer^  updateTimer;
		System::Windows::Forms::Label^  statusLbl;
		System::Windows::Forms::Panel^  vuPanel1;
		System::Windows::Forms::Panel^  vuMeterR;
		System::Windows::Forms::Panel^  vuMeterL;
		System::Windows::Forms::Panel^  vuBack2;
		System::Windows::Forms::Panel^  vuBack1;
		System::Windows::Forms::Label^  label9;
		System::Windows::Forms::Label^  label8;
		System::Windows::Forms::Label^  label7;
		System::Windows::Forms::Button^  peakR;
		System::Windows::Forms::Button^  peakL;
		System::Windows::Forms::Panel^  barPosBack;
		System::Windows::Forms::Panel^  barPos;
		System::Windows::Forms::Label^  trackPosLbl;
		System::Windows::Forms::Panel^  backPanel;
		System::Windows::Forms::Panel^  equPanel;
		System::Windows::Forms::TrackBar^  b0;
		System::Windows::Forms::TrackBar^  b8;
		System::Windows::Forms::TrackBar^  b7;
		System::Windows::Forms::TrackBar^  b6;
		System::Windows::Forms::TrackBar^  b5;
		System::Windows::Forms::TrackBar^  b4;
		System::Windows::Forms::TrackBar^  b3;
		System::Windows::Forms::TrackBar^  b2;
		System::Windows::Forms::TrackBar^  b1;
		System::Windows::Forms::Label^  label10;
		System::Windows::Forms::CheckBox^  equCheck;
		System::Windows::Forms::Button^  equFlatBtn;
		System::Windows::Forms::Panel^  masterPanel;
		System::Windows::Forms::Label^  label11;
		System::Windows::Forms::Label^  masterVolLbl;
		System::Windows::Forms::TrackBar^  rightVolBar;
		System::Windows::Forms::TrackBar^  leftVolBar;
		System::Windows::Forms::Label^  label12;
		System::Windows::Forms::Label^  rightVolLbl;
		System::Windows::Forms::Label^  leftVolLbl;
		System::Windows::Forms::TrackBar^  rateValBar;
		System::Windows::Forms::TrackBar^  tempoValBar;
		System::Windows::Forms::TrackBar^  pitchValBar;
		System::Windows::Forms::Label^  label13;
		System::Windows::Forms::Label^  rateLbl;
		System::Windows::Forms::Label^  tempoLbl;
		System::Windows::Forms::Label^  pitchLbl;
		System::Windows::Forms::TrackBar^  masterVolBar;
		System::Windows::Forms::Button^  nextTrackBtn;
		System::Windows::Forms::Button^  prevTrackBtn;
		System::Windows::Forms::Label^  currentTimeLbl;
#pragma endregion				 


		/// Read resource file settings
		void resourceRead()
		{
			if (!File::Exists("./PlayerForm.resx")) { return; }
			int counter = 0;
			int vol;
			Double val = 0;
			for each (DictionaryEntry res in resReader)
			{
				switch (counter)
				{
				case 0:
					Double::TryParse(res.Value->ToString(), val);
					b0->Value = val;
					break;
				case 1:
					Double::TryParse(res.Value->ToString(), val);
					b1->Value = val;
					break;
				case 2:
					Double::TryParse(res.Value->ToString(), val);
					b2->Value = val;
					break;
				case 3:
					Double::TryParse(res.Value->ToString(), val);
					b3->Value = val;
					break;
				case 4:
					Double::TryParse(res.Value->ToString(), val);
					b4->Value = val;
					break;
				case 5:
					Double::TryParse(res.Value->ToString(), val);
					b5->Value = val;
					break;
				case 6:
					Double::TryParse(res.Value->ToString(), val);
					b6->Value = val;
					break;
				case 7:
					Double::TryParse(res.Value->ToString(), val);
					b7->Value = val;
					break;
				case 8:
					Double::TryParse(res.Value->ToString(), val);
					b8->Value = val;
					break;
				case 9:
					soundsDirPath = res.Value->ToString();
					break;

				case 10:
					bool enbl;
					Boolean::TryParse(res.Value->ToString(), enbl);
					equalizerEnable(enbl);
					break;
				case 11:
					vol;
					int::TryParse(res.Value->ToString(), vol);
					setMasterVolume(vol);
					break;
				case 12:
					vol;
					int::TryParse(res.Value->ToString(), vol);
					setLeftRightVolume(vol, rightVolume);
					break;
				case 13:
					vol;
					int::TryParse(res.Value->ToString(), vol);
					setLeftRightVolume(leftVolume, vol);
					break;
				case 14:
					vol;
					int::TryParse(res.Value->ToString(), vol);
					setPTR('P', vol);
					break;
				case 15:
					vol;
					int::TryParse(res.Value->ToString(), vol);
					setPTR('T', vol);
					break;
				case 16:
					vol;
					int::TryParse(res.Value->ToString(), vol);
					setPTR('R', vol);
					break;

				}
				counter++;
			}

		}


		/// Write resource file settings
		void resourceWrite()
		{
			for (int i = 0; i < resourcArr->Length; i++)
			{
				String^ key = resourcArr[i]->ToString();
				switch (i)
				{
				case 0:
					resWriter->AddResource(gcnew ResXDataNode(key, b0->Value.ToString()));
					break;
				case 1:
					resWriter->AddResource(gcnew ResXDataNode(key, b1->Value.ToString()));
					break;
				case 2:
					resWriter->AddResource(gcnew ResXDataNode(key, b2->Value.ToString()));

					break;
				case 3:
					resWriter->AddResource(gcnew ResXDataNode(key, b3->Value.ToString()));
					break;
				case 4:
					resWriter->AddResource(gcnew ResXDataNode(key, b4->Value.ToString()));
					break;
				case 5:
					resWriter->AddResource(gcnew ResXDataNode(key, b5->Value.ToString()));
					break;
				case 6:
					resWriter->AddResource(gcnew ResXDataNode(key, b6->Value.ToString()));
					break;
				case 7:
					resWriter->AddResource(gcnew ResXDataNode(key, b7->Value.ToString()));
					break;
				case 8:
					resWriter->AddResource(gcnew ResXDataNode(key, b8->Value.ToString()));
					break;
				case 9:
					resWriter->AddResource(gcnew ResXDataNode(key, soundsDirPath));
					break;
				case 10:
					resWriter->AddResource(gcnew ResXDataNode(key, equCheck->Checked.ToString()));
					break;
				case 11:
					resWriter->AddResource(gcnew ResXDataNode(key, masterVolume.ToString()));
					break;
				case 12:
					resWriter->AddResource(gcnew ResXDataNode(key, leftVolume.ToString()));
					break;
				case 13:
					resWriter->AddResource(gcnew ResXDataNode(key, rightVolume.ToString()));
					break;
				case 14:
					resWriter->AddResource(gcnew ResXDataNode(key, pitchValBar->Value.ToString()));
					break;
				case 15:
					resWriter->AddResource(gcnew ResXDataNode(key, tempoValBar->Value.ToString()));
					break;
				case 16:
					resWriter->AddResource(gcnew ResXDataNode(key, rateValBar->Value.ToString()));
					break;
				}

			}
			resWriter->Generate();
			resWriter->Close();
		}

		/// Load all tracks in list of selected folder
		void loadTracks()
		{
			if (!Directory::Exists(soundsDirPath)) { return; }
			trackList->Items->Clear();
			int filesCount = Directory::GetFiles(soundsDirPath)->Length;
			for (int i = 0; i < filesCount; i++)
			{
				String^ file = Directory::GetFiles(soundsDirPath)[i]->ToString();
				String^ fileName = FileInfo::FileInfo(file).Name;
				String^ ext = FileInfo::FileInfo(file).Extension;
				if (ext == ".mp3" || ext == ".wav")
				{
					trackList->Items->Add(fileName);
				}

			}
			pathLbl->Text = soundsDirPath;
		}

		/// Load track file
		void loadTrackFile(String^ track)
		{
			player->OpenFile(track, TStreamFormat::sfAutodetect);
			loadID3(track);
			getLeght();
		}

		/// Start player
		void startPlayer()
		{
			if (!updateTimer->Enabled) { updateTimer->Start(); }
			trackLbl->Text = trackName;
			player->GetPosition(curPositionTrack);
			player->PlayLoop(TTimeFormat::tfSecond, curPositionTrack, TTimeFormat::tfSecond, totalLenghtTrack, 1, false);
			playStopBtn->Text = "Stop";
			statusLbl->Text = "PLAY";
			player->GetStatus(statusInfo);
		}

		/// Stop player
		void stopPlayer()
		{
			if (updateTimer->Enabled) { updateTimer->Stop(); }
			vuMeterOff();
			player->StopPlayback();
			playStopBtn->Text = "Play";
			statusLbl->Text = "STOP";
			statusLbl->Visible = true;
		}

		/// Pause player
		void pausePlayer()
		{
			player->GetStatus(statusInfo);
			if (statusInfo.fPause || !statusInfo.fPlay) { return; }
			player->PausePlayback();
			player->GetStatus(statusInfo);
			playStopBtn->Text = "Play";
			statusLbl->Text = "PAUSE";
			vuMeterOff();
		}

		/// Next track load and play
		void nextTrack()
		{
			if (trackList->Items->Count == 0) { return; }
			stopPlayer();
			trackListIndex++;
			if (trackListIndex >= trackList->Items->Count - 1) { trackListIndex = 0; }
			trackList->SelectedIndex = trackListIndex;
			trackName = trackList->SelectedItem->ToString();
			playingTrack = soundsDirPath + "/" + trackName;
			loadTrackFile(playingTrack);
			startPlayer();
		}

		/// Previous track load and play
		void previousTrack()
		{
			if (trackList->Items->Count == 0) { return; }
			stopPlayer();
			trackListIndex--;
			if (trackListIndex < 0) { trackListIndex = trackList->Items->Count - 1; }
			trackList->SelectedIndex = trackListIndex;
			trackName = trackList->SelectedItem->ToString();
			playingTrack = soundsDirPath + "/" + trackName;
			loadTrackFile(playingTrack);
			startPlayer();
		}

		/// Load track information
		void loadID3(String^ track)
		{
			clearLabelsInfo();
			libZPlay::TID3InfoEx info;
			if (player->LoadID3Ex(info, true))
			{
				titleLbl->Text = info.Title;
				artistLbl->Text = info.Artist;
				yearLbl->Text = info.Year;
			}

		}

		/// VuMeter
		void vuMeter()
		{
			if (player != nullptr && !statusInfo.fPause)
			{
				player->GetVUData(leftChannelVu, rightChannelVu);
				vuMeterL->Height = 200 - (leftChannelVu * 2);
				vuMeterR->Height = 200 - (rightChannelVu * 2);
				if (leftChannelVu > 85)
				{
					if (leftChannelVu > 95) { peakL->BackColor = Color::Red; }
					else { peakL->BackColor = Color::Yellow; }
				}
				else { peakL->BackColor = Color::Black; }

				if (rightChannelVu > 85)
				{
					if (rightChannelVu > 95) { peakR->BackColor = Color::Red; }
					else { peakR->BackColor = Color::Yellow; }
				}
				else { peakR->BackColor = Color::Black; }

			}
		}

		/// Off vu meter
		void vuMeterOff()
		{
			vuMeterL->Height = 200;
			vuMeterR->Height = 200;
			peakL->BackColor = Color::Black;
			peakR->BackColor = Color::Black;
		}

		/// Get track  position		
		void getPosition()
		{
			player->GetPosition(timeInfo);
			player->GetStatus(statusInfo);
			int hour = timeInfo.hms.hour;
			int min = timeInfo.hms.minute;
			int sec = timeInfo.hms.second;
			H = hour > 9 ? "" : "0";
			M = min > 9 ? "" : "0";
			S = sec > 9 ? "" : "0";
			currentTimeLbl->Text = H + hour + "." + M + min + "." + S + sec;
			if (timeInfo.sec == 0 && !statusInfo.fPlay && !statusInfo.fPause) { updateTimer->Stop(); nextTrack(); }
			setTrackBar();
		}

		/// Set track new position
		void setTrackPosition()
		{
			double cursorPosX = barPos->PointToClient(Cursor->Position).X;
			double newPos = (cursorPosX / 300) *totalLenghtTrack.sec;
			TStreamTime time;
			time.sec = newPos;
			player->Seek(TTimeFormat::tfSecond, time, TSeekMethod::smFromBeginning);
		}

		/// Set bar position of track
		void setTrackBar()
		{
			double curTime = timeInfo.sec;
			double totalTime = totalLenghtTrack.sec;
			double pos = (curTime / totalTime) * 300;
			double posPerc = (curTime / totalTime) * 100;
			barPos->Width = pos;
			trackPosLbl->Text = Math::Round(posPerc) + "%";
		}

		/// Get track lenght
		void getLeght()
		{
			player->GetStreamInfo(streamInfo);
			int hour = streamInfo.Length.hms.hour;
			int min = streamInfo.Length.hms.minute;
			int sec = streamInfo.Length.hms.second;
			String^ H;
			String^ M;
			String^ S;
			H = hour > 9 ? "" : "0";
			M = min > 9 ? "" : "0";
			S = sec > 9 ? "" : "0";
			totalTimeLbl->Text = H + hour + "." + M + min + "." + S + sec;
			totalLenghtTrack = streamInfo.Length;
		}

		/// Clear all label info
		void clearLabelsInfo()
		{
			trackLbl->Text = "";
			artistLbl->Text = "";
			titleLbl->Text = "";
			yearLbl->Text = "";
			currentTimeLbl->Text = "";
			totalTimeLbl->Text = "";
			statusLbl->Text = "";
		}

		/// Convert std::string to System::String
		String^ convertToString(std::string str)
		{
			return gcnew String(str.c_str());
		}

		/// Equalizer enable disable
		void equalizerEnable(bool enable)
		{
			player->EnableEqualizer(enable);
			equCheck->Checked = enable;
		}

		/// Equalizer set band parameters
		void equalizerSet()
		{
			array<int> ^freqBands = gcnew array<int>(freqBandsCount) { 100, 200, 300, 1000, 2000, 3000, 5000, 7000, 12000 };
			if (player != nullptr)
			{
				player->SetEqualizerPoints(freqBands, 9);
			}
		}

		/// Equalizer change
		void equalizerChange(int band, int value)
		{
			player->SetEqualizerBandGain(band, value);			
		}

		/// Equalizer flat
		void equalizerFlat()
		{
			for each (Control^ control in equPanel->Controls)
			{
				if (control->Name->IndexOf("b") == 0)
				{
					int numBand = 0;
					int::TryParse(control->Name->Substring(1), numBand);
					TrackBar^ bar = dynamic_cast<TrackBar^> (control);
					bar->Value = 0;
				}

			}
		}

		/// Set master Volume
		void setMasterVolume(int vol)
		{
			player->SetMasterVolume(vol, vol);
			masterVolume = vol;
			masterVolLbl->Text = vol.ToString();
			masterVolBar->Value = vol;
		}

		/// Set Left Right volume
		void setLeftRightVolume(int left, int right)
		{
			player->SetPlayerVolume(left, right);
			leftVolume = left;
			rightVolume = right;
			leftVolBar->Value = left;
			rightVolBar->Value = right;
			leftVolLbl->Text = left.ToString();
			rightVolLbl->Text = right.ToString();
		}

		/// Set PTR (Pitch Tempo Rate)
		void setPTR(char cmd, int val)
		{
			switch (cmd)
			{
			case 'P':
				if (player->GetPitch() == 100 && val == 100) { return; }
				player->SetPitch(val);
				pitchValBar->Value = val;
				pitchLbl->Text = val.ToString();
				break;
			case 'T':
				if (player->GetTempo() == 100 && val == 100) { return; }
				player->SetTempo(val);
				tempoValBar->Value = val;
				tempoLbl->Text = val.ToString();
				break;
			case 'R':
				if (player->GetRate() == 100 && val == 100) { return; }
				player->SetRate(val);
				rateValBar->Value = val;
				rateLbl->Text = val.ToString();
				break;
			}

		}

		/// Reset
		void reset()
		{
			masterVolume = 100;
			leftVolume = 100;
			rightVolume = 100;
			equalizerFlat();
			masterVolBar->Value = masterVolume;
			setMasterVolume(masterVolume);
			setLeftRightVolume(leftVolume, rightVolume);
		}

#pragma region  print function ovverloading
		//Print in outpout window
		void print(System::String^  arg)
		{
			Diagnostics::Debug::WriteLine(arg);
		}
		void print(int  arg)
		{
			Diagnostics::Debug::WriteLine(arg.ToString());
		}
		void print(double  arg)
		{
			Diagnostics::Debug::WriteLine(arg.ToString());
		}

#pragma endregion


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->trackList = (gcnew System::Windows::Forms::ListBox());
			this->folderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->dirBtn = (gcnew System::Windows::Forms::Button());
			this->pathLbl = (gcnew System::Windows::Forms::Label());
			this->playBtnPanel = (gcnew System::Windows::Forms::Panel());
			this->trackPosLbl = (gcnew System::Windows::Forms::Label());
			this->barPos = (gcnew System::Windows::Forms::Panel());
			this->barPosBack = (gcnew System::Windows::Forms::Panel());
			this->statusLbl = (gcnew System::Windows::Forms::Label());
			this->totalTimeLbl = (gcnew System::Windows::Forms::Label());
			this->currentTimeLbl = (gcnew System::Windows::Forms::Label());
			this->yearLbl = (gcnew System::Windows::Forms::Label());
			this->artistLbl = (gcnew System::Windows::Forms::Label());
			this->titleLbl = (gcnew System::Windows::Forms::Label());
			this->trackLbl = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->pauseBtn = (gcnew System::Windows::Forms::Button());
			this->playStopBtn = (gcnew System::Windows::Forms::Button());
			this->updateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->vuPanel1 = (gcnew System::Windows::Forms::Panel());
			this->peakR = (gcnew System::Windows::Forms::Button());
			this->peakL = (gcnew System::Windows::Forms::Button());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->vuMeterR = (gcnew System::Windows::Forms::Panel());
			this->vuMeterL = (gcnew System::Windows::Forms::Panel());
			this->vuBack2 = (gcnew System::Windows::Forms::Panel());
			this->vuBack1 = (gcnew System::Windows::Forms::Panel());
			this->backPanel = (gcnew System::Windows::Forms::Panel());
			this->equPanel = (gcnew System::Windows::Forms::Panel());
			this->equFlatBtn = (gcnew System::Windows::Forms::Button());
			this->equCheck = (gcnew System::Windows::Forms::CheckBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->b8 = (gcnew System::Windows::Forms::TrackBar());
			this->b7 = (gcnew System::Windows::Forms::TrackBar());
			this->b6 = (gcnew System::Windows::Forms::TrackBar());
			this->b5 = (gcnew System::Windows::Forms::TrackBar());
			this->b4 = (gcnew System::Windows::Forms::TrackBar());
			this->b3 = (gcnew System::Windows::Forms::TrackBar());
			this->b2 = (gcnew System::Windows::Forms::TrackBar());
			this->b1 = (gcnew System::Windows::Forms::TrackBar());
			this->b0 = (gcnew System::Windows::Forms::TrackBar());
			this->masterPanel = (gcnew System::Windows::Forms::Panel());
			this->rateLbl = (gcnew System::Windows::Forms::Label());
			this->tempoLbl = (gcnew System::Windows::Forms::Label());
			this->pitchLbl = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->rateValBar = (gcnew System::Windows::Forms::TrackBar());
			this->tempoValBar = (gcnew System::Windows::Forms::TrackBar());
			this->pitchValBar = (gcnew System::Windows::Forms::TrackBar());
			this->rightVolLbl = (gcnew System::Windows::Forms::Label());
			this->leftVolLbl = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->rightVolBar = (gcnew System::Windows::Forms::TrackBar());
			this->leftVolBar = (gcnew System::Windows::Forms::TrackBar());
			this->masterVolLbl = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->masterVolBar = (gcnew System::Windows::Forms::TrackBar());
			this->prevTrackBtn = (gcnew System::Windows::Forms::Button());
			this->nextTrackBtn = (gcnew System::Windows::Forms::Button());
			this->playBtnPanel->SuspendLayout();
			this->vuPanel1->SuspendLayout();
			this->backPanel->SuspendLayout();
			this->equPanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b0))->BeginInit();
			this->masterPanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rateValBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempoValBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pitchValBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rightVolBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->leftVolBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->masterVolBar))->BeginInit();
			this->SuspendLayout();
			// 
			// trackList
			// 
			this->trackList->BackColor = System::Drawing::Color::SteelBlue;
			this->trackList->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->trackList->ForeColor = System::Drawing::Color::AliceBlue;
			this->trackList->FormattingEnabled = true;
			this->trackList->HorizontalScrollbar = true;
			this->trackList->ItemHeight = 16;
			this->trackList->Location = System::Drawing::Point(2, 187);
			this->trackList->Name = L"trackList";
			this->trackList->Size = System::Drawing::Size(464, 244);
			this->trackList->TabIndex = 0;
			this->trackList->SelectedIndexChanged += gcnew System::EventHandler(this, &PlayerForm::trackList_SelectedIndexChanged);
			this->trackList->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &PlayerForm::trackList_MouseDoubleClick);
			// 
			// dirBtn
			// 
			this->dirBtn->Location = System::Drawing::Point(2, 158);
			this->dirBtn->Name = L"dirBtn";
			this->dirBtn->Size = System::Drawing::Size(95, 23);
			this->dirBtn->TabIndex = 1;
			this->dirBtn->Text = L"Sounds Folder";
			this->dirBtn->UseVisualStyleBackColor = true;
			this->dirBtn->Click += gcnew System::EventHandler(this, &PlayerForm::dirBtn_Click);
			// 
			// pathLbl
			// 
			this->pathLbl->AutoSize = true;
			this->pathLbl->Location = System::Drawing::Point(102, 165);
			this->pathLbl->Name = L"pathLbl";
			this->pathLbl->Size = System::Drawing::Size(35, 13);
			this->pathLbl->TabIndex = 2;
			this->pathLbl->Text = L"Path=";
			// 
			// playBtnPanel
			// 
			this->playBtnPanel->BackColor = System::Drawing::Color::SteelBlue;
			this->playBtnPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->playBtnPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->playBtnPanel->Controls->Add(this->trackPosLbl);
			this->playBtnPanel->Controls->Add(this->barPos);
			this->playBtnPanel->Controls->Add(this->barPosBack);
			this->playBtnPanel->Controls->Add(this->statusLbl);
			this->playBtnPanel->Controls->Add(this->totalTimeLbl);
			this->playBtnPanel->Controls->Add(this->currentTimeLbl);
			this->playBtnPanel->Controls->Add(this->yearLbl);
			this->playBtnPanel->Controls->Add(this->artistLbl);
			this->playBtnPanel->Controls->Add(this->titleLbl);
			this->playBtnPanel->Controls->Add(this->trackLbl);
			this->playBtnPanel->Controls->Add(this->label6);
			this->playBtnPanel->Controls->Add(this->label5);
			this->playBtnPanel->Controls->Add(this->label4);
			this->playBtnPanel->Controls->Add(this->label3);
			this->playBtnPanel->Controls->Add(this->label2);
			this->playBtnPanel->Controls->Add(this->label1);
			this->playBtnPanel->Location = System::Drawing::Point(2, 4);
			this->playBtnPanel->Name = L"playBtnPanel";
			this->playBtnPanel->Size = System::Drawing::Size(464, 148);
			this->playBtnPanel->TabIndex = 3;
			// 
			// trackPosLbl
			// 
			this->trackPosLbl->AutoSize = true;
			this->trackPosLbl->BackColor = System::Drawing::Color::Transparent;
			this->trackPosLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->trackPosLbl->ForeColor = System::Drawing::Color::Azure;
			this->trackPosLbl->Location = System::Drawing::Point(426, 119);
			this->trackPosLbl->Name = L"trackPosLbl";
			this->trackPosLbl->Size = System::Drawing::Size(35, 14);
			this->trackPosLbl->TabIndex = 15;
			this->trackPosLbl->Text = L"100%";
			// 
			// barPos
			// 
			this->barPos->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->barPos->Cursor = System::Windows::Forms::Cursors::Hand;
			this->barPos->Location = System::Drawing::Point(124, 119);
			this->barPos->Name = L"barPos";
			this->barPos->Size = System::Drawing::Size(300, 12);
			this->barPos->TabIndex = 14;
			this->barPos->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &PlayerForm::barPosBack_MouseClick);
			// 
			// barPosBack
			// 
			this->barPosBack->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->barPosBack->Cursor = System::Windows::Forms::Cursors::Hand;
			this->barPosBack->Location = System::Drawing::Point(122, 116);
			this->barPosBack->Name = L"barPosBack";
			this->barPosBack->Size = System::Drawing::Size(305, 17);
			this->barPosBack->TabIndex = 13;
			this->barPosBack->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &PlayerForm::barPosBack_MouseClick);
			// 
			// statusLbl
			// 
			this->statusLbl->AutoSize = true;
			this->statusLbl->BackColor = System::Drawing::Color::Transparent;
			this->statusLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->statusLbl->ForeColor = System::Drawing::Color::Honeydew;
			this->statusLbl->Location = System::Drawing::Point(9, 59);
			this->statusLbl->Name = L"statusLbl";
			this->statusLbl->Size = System::Drawing::Size(58, 24);
			this->statusLbl->TabIndex = 12;
			this->statusLbl->Text = L"PLAY";
			// 
			// totalTimeLbl
			// 
			this->totalTimeLbl->AutoSize = true;
			this->totalTimeLbl->BackColor = System::Drawing::Color::Transparent;
			this->totalTimeLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->totalTimeLbl->ForeColor = System::Drawing::Color::Azure;
			this->totalTimeLbl->Location = System::Drawing::Point(7, 115);
			this->totalTimeLbl->Name = L"totalTimeLbl";
			this->totalTimeLbl->Size = System::Drawing::Size(81, 19);
			this->totalTimeLbl->TabIndex = 11;
			this->totalTimeLbl->Text = L"00:00:00";
			// 
			// currentTimeLbl
			// 
			this->currentTimeLbl->AutoSize = true;
			this->currentTimeLbl->BackColor = System::Drawing::Color::Transparent;
			this->currentTimeLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->currentTimeLbl->ForeColor = System::Drawing::Color::Azure;
			this->currentTimeLbl->Location = System::Drawing::Point(8, 23);
			this->currentTimeLbl->Name = L"currentTimeLbl";
			this->currentTimeLbl->Size = System::Drawing::Size(81, 19);
			this->currentTimeLbl->TabIndex = 10;
			this->currentTimeLbl->Text = L"00:00:00";
			// 
			// yearLbl
			// 
			this->yearLbl->AutoSize = true;
			this->yearLbl->BackColor = System::Drawing::Color::Transparent;
			this->yearLbl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->yearLbl->ForeColor = System::Drawing::Color::Azure;
			this->yearLbl->Location = System::Drawing::Point(215, 96);
			this->yearLbl->Name = L"yearLbl";
			this->yearLbl->Size = System::Drawing::Size(23, 17);
			this->yearLbl->TabIndex = 9;
			this->yearLbl->Text = L"---";
			// 
			// artistLbl
			// 
			this->artistLbl->AutoSize = true;
			this->artistLbl->BackColor = System::Drawing::Color::Transparent;
			this->artistLbl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->artistLbl->ForeColor = System::Drawing::Color::Azure;
			this->artistLbl->Location = System::Drawing::Point(215, 66);
			this->artistLbl->Name = L"artistLbl";
			this->artistLbl->Size = System::Drawing::Size(23, 17);
			this->artistLbl->TabIndex = 8;
			this->artistLbl->Text = L"---";
			// 
			// titleLbl
			// 
			this->titleLbl->AutoSize = true;
			this->titleLbl->BackColor = System::Drawing::Color::Transparent;
			this->titleLbl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->titleLbl->ForeColor = System::Drawing::Color::Azure;
			this->titleLbl->Location = System::Drawing::Point(215, 36);
			this->titleLbl->Name = L"titleLbl";
			this->titleLbl->Size = System::Drawing::Size(23, 17);
			this->titleLbl->TabIndex = 7;
			this->titleLbl->Text = L"---";
			// 
			// trackLbl
			// 
			this->trackLbl->AutoSize = true;
			this->trackLbl->BackColor = System::Drawing::Color::Transparent;
			this->trackLbl->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->trackLbl->ForeColor = System::Drawing::Color::Azure;
			this->trackLbl->Location = System::Drawing::Point(215, 7);
			this->trackLbl->Name = L"trackLbl";
			this->trackLbl->Size = System::Drawing::Size(19, 15);
			this->trackLbl->TabIndex = 6;
			this->trackLbl->Text = L"---";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::Transparent;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label6->ForeColor = System::Drawing::Color::LightBlue;
			this->label6->Location = System::Drawing::Point(8, 98);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(75, 17);
			this->label6->TabIndex = 5;
			this->label6->Text = L"Total Time";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::Transparent;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label5->ForeColor = System::Drawing::Color::LightBlue;
			this->label5->Location = System::Drawing::Point(8, 6);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(90, 17);
			this->label5->TabIndex = 4;
			this->label5->Text = L"Current Time";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::Transparent;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label4->ForeColor = System::Drawing::Color::LightBlue;
			this->label4->Location = System::Drawing::Point(167, 96);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(42, 17);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Year:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::Transparent;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label3->ForeColor = System::Drawing::Color::LightBlue;
			this->label3->Location = System::Drawing::Point(166, 66);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(44, 17);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Artist:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label2->ForeColor = System::Drawing::Color::LightBlue;
			this->label2->Location = System::Drawing::Point(170, 36);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(39, 17);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Title:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label1->ForeColor = System::Drawing::Color::LightBlue;
			this->label1->Location = System::Drawing::Point(122, 6);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(87, 17);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Track name:";
			// 
			// pauseBtn
			// 
			this->pauseBtn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->pauseBtn->Location = System::Drawing::Point(87, 3);
			this->pauseBtn->Name = L"pauseBtn";
			this->pauseBtn->Size = System::Drawing::Size(75, 35);
			this->pauseBtn->TabIndex = 1;
			this->pauseBtn->Text = L"Pause";
			this->pauseBtn->UseVisualStyleBackColor = true;
			this->pauseBtn->Click += gcnew System::EventHandler(this, &PlayerForm::pauseBtn_Click);
			// 
			// playStopBtn
			// 
			this->playStopBtn->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->playStopBtn->Location = System::Drawing::Point(3, 3);
			this->playStopBtn->Name = L"playStopBtn";
			this->playStopBtn->Size = System::Drawing::Size(75, 35);
			this->playStopBtn->TabIndex = 0;
			this->playStopBtn->Text = L"Play";
			this->playStopBtn->UseVisualStyleBackColor = true;
			this->playStopBtn->Click += gcnew System::EventHandler(this, &PlayerForm::playStopBtn_Click);
			// 
			// updateTimer
			// 
			this->updateTimer->Tick += gcnew System::EventHandler(this, &PlayerForm::updateTimer_Tick);
			// 
			// vuPanel1
			// 
			this->vuPanel1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->vuPanel1->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->vuPanel1->Controls->Add(this->peakR);
			this->vuPanel1->Controls->Add(this->peakL);
			this->vuPanel1->Controls->Add(this->label9);
			this->vuPanel1->Controls->Add(this->label8);
			this->vuPanel1->Controls->Add(this->label7);
			this->vuPanel1->Controls->Add(this->vuMeterR);
			this->vuPanel1->Controls->Add(this->vuMeterL);
			this->vuPanel1->Controls->Add(this->vuBack2);
			this->vuPanel1->Controls->Add(this->vuBack1);
			this->vuPanel1->Location = System::Drawing::Point(803, 187);
			this->vuPanel1->Name = L"vuPanel1";
			this->vuPanel1->Size = System::Drawing::Size(75, 244);
			this->vuPanel1->TabIndex = 4;
			// 
			// peakR
			// 
			this->peakR->BackColor = System::Drawing::Color::Black;
			this->peakR->Enabled = false;
			this->peakR->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->peakR->Location = System::Drawing::Point(45, 2);
			this->peakR->Name = L"peakR";
			this->peakR->Size = System::Drawing::Size(20, 15);
			this->peakR->TabIndex = 16;
			this->peakR->UseVisualStyleBackColor = false;
			// 
			// peakL
			// 
			this->peakL->BackColor = System::Drawing::Color::Black;
			this->peakL->Enabled = false;
			this->peakL->ForeColor = System::Drawing::SystemColors::ControlDarkDark;
			this->peakL->Location = System::Drawing::Point(6, 2);
			this->peakL->Name = L"peakL";
			this->peakL->Size = System::Drawing::Size(20, 15);
			this->peakL->TabIndex = 5;
			this->peakL->UseVisualStyleBackColor = false;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->BackColor = System::Drawing::Color::Transparent;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label9->ForeColor = System::Drawing::Color::LightBlue;
			this->label9->Location = System::Drawing::Point(48, 15);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(16, 15);
			this->label9->TabIndex = 15;
			this->label9->Text = L"R";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::Transparent;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label8->ForeColor = System::Drawing::Color::LightBlue;
			this->label8->Location = System::Drawing::Point(10, 15);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(14, 15);
			this->label8->TabIndex = 14;
			this->label8->Text = L"L";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::Transparent;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label7->ForeColor = System::Drawing::Color::LightBlue;
			this->label7->Location = System::Drawing::Point(25, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(23, 15);
			this->label7->TabIndex = 13;
			this->label7->Text = L"VU";
			// 
			// vuMeterR
			// 
			this->vuMeterR->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->vuMeterR->Location = System::Drawing::Point(42, 40);
			this->vuMeterR->Name = L"vuMeterR";
			this->vuMeterR->Size = System::Drawing::Size(24, 200);
			this->vuMeterR->TabIndex = 2;
			// 
			// vuMeterL
			// 
			this->vuMeterL->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->vuMeterL->Location = System::Drawing::Point(7, 40);
			this->vuMeterL->Name = L"vuMeterL";
			this->vuMeterL->Size = System::Drawing::Size(24, 200);
			this->vuMeterL->TabIndex = 1;
			// 
			// vuBack2
			// 
			this->vuBack2->BackColor = System::Drawing::SystemColors::MenuHighlight;
			this->vuBack2->Location = System::Drawing::Point(42, 40);
			this->vuBack2->Name = L"vuBack2";
			this->vuBack2->Size = System::Drawing::Size(24, 200);
			this->vuBack2->TabIndex = 1;
			// 
			// vuBack1
			// 
			this->vuBack1->BackColor = System::Drawing::SystemColors::MenuHighlight;
			this->vuBack1->Location = System::Drawing::Point(7, 40);
			this->vuBack1->Name = L"vuBack1";
			this->vuBack1->Size = System::Drawing::Size(24, 200);
			this->vuBack1->TabIndex = 0;
			// 
			// backPanel
			// 
			this->backPanel->BackColor = System::Drawing::Color::SteelBlue;
			this->backPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->backPanel->Controls->Add(this->nextTrackBtn);
			this->backPanel->Controls->Add(this->prevTrackBtn);
			this->backPanel->Controls->Add(this->equPanel);
			this->backPanel->Controls->Add(this->pauseBtn);
			this->backPanel->Controls->Add(this->playStopBtn);
			this->backPanel->Location = System::Drawing::Point(470, 4);
			this->backPanel->Name = L"backPanel";
			this->backPanel->Size = System::Drawing::Size(408, 149);
			this->backPanel->TabIndex = 5;
			// 
			// equPanel
			// 
			this->equPanel->BackColor = System::Drawing::Color::Black;
			this->equPanel->Controls->Add(this->equFlatBtn);
			this->equPanel->Controls->Add(this->equCheck);
			this->equPanel->Controls->Add(this->label10);
			this->equPanel->Controls->Add(this->b8);
			this->equPanel->Controls->Add(this->b7);
			this->equPanel->Controls->Add(this->b6);
			this->equPanel->Controls->Add(this->b5);
			this->equPanel->Controls->Add(this->b4);
			this->equPanel->Controls->Add(this->b3);
			this->equPanel->Controls->Add(this->b2);
			this->equPanel->Controls->Add(this->b1);
			this->equPanel->Controls->Add(this->b0);
			this->equPanel->Location = System::Drawing::Point(3, 40);
			this->equPanel->Name = L"equPanel";
			this->equPanel->Size = System::Drawing::Size(402, 109);
			this->equPanel->TabIndex = 2;
			// 
			// equFlatBtn
			// 
			this->equFlatBtn->Font = (gcnew System::Drawing::Font(L"Consolas", 2.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Millimeter,
				static_cast<System::Byte>(161)));
			this->equFlatBtn->Location = System::Drawing::Point(350, 90);
			this->equFlatBtn->Name = L"equFlatBtn";
			this->equFlatBtn->Size = System::Drawing::Size(50, 18);
			this->equFlatBtn->TabIndex = 18;
			this->equFlatBtn->Text = L"Flat";
			this->equFlatBtn->UseVisualStyleBackColor = true;
			this->equFlatBtn->Click += gcnew System::EventHandler(this, &PlayerForm::equFlatBtn_Click);
			// 
			// equCheck
			// 
			this->equCheck->AutoSize = true;
			this->equCheck->ForeColor = System::Drawing::SystemColors::ActiveCaption;
			this->equCheck->Location = System::Drawing::Point(4, 90);
			this->equCheck->Name = L"equCheck";
			this->equCheck->Size = System::Drawing::Size(105, 17);
			this->equCheck->TabIndex = 17;
			this->equCheck->Text = L"Enable Equalizer";
			this->equCheck->UseVisualStyleBackColor = true;
			this->equCheck->CheckedChanged += gcnew System::EventHandler(this, &PlayerForm::equCheck_CheckedChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::Transparent;
			this->label10->Font = (gcnew System::Drawing::Font(L"Consolas", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label10->ForeColor = System::Drawing::Color::Azure;
			this->label10->Location = System::Drawing::Point(25, 3);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(355, 13);
			this->label10->TabIndex = 16;
			this->label10->Text = L"100    200    300   1000   2000  3000   5000  7000   12000";
			// 
			// b8
			// 
			this->b8->Location = System::Drawing::Point(338, 9);
			this->b8->Maximum = 12;
			this->b8->Minimum = -12;
			this->b8->Name = L"b8";
			this->b8->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b8->Size = System::Drawing::Size(45, 83);
			this->b8->TabIndex = 8;
			this->b8->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b8->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// b7
			// 
			this->b7->Location = System::Drawing::Point(298, 9);
			this->b7->Maximum = 12;
			this->b7->Minimum = -12;
			this->b7->Name = L"b7";
			this->b7->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b7->Size = System::Drawing::Size(45, 83);
			this->b7->TabIndex = 7;
			this->b7->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b7->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// b6
			// 
			this->b6->Location = System::Drawing::Point(258, 9);
			this->b6->Maximum = 12;
			this->b6->Minimum = -12;
			this->b6->Name = L"b6";
			this->b6->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b6->Size = System::Drawing::Size(45, 83);
			this->b6->TabIndex = 6;
			this->b6->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b6->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// b5
			// 
			this->b5->Location = System::Drawing::Point(218, 9);
			this->b5->Maximum = 12;
			this->b5->Minimum = -12;
			this->b5->Name = L"b5";
			this->b5->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b5->Size = System::Drawing::Size(45, 83);
			this->b5->TabIndex = 5;
			this->b5->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b5->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// b4
			// 
			this->b4->Location = System::Drawing::Point(178, 9);
			this->b4->Maximum = 12;
			this->b4->Minimum = -12;
			this->b4->Name = L"b4";
			this->b4->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b4->Size = System::Drawing::Size(45, 83);
			this->b4->TabIndex = 4;
			this->b4->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b4->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// b3
			// 
			this->b3->Location = System::Drawing::Point(138, 9);
			this->b3->Maximum = 12;
			this->b3->Minimum = -12;
			this->b3->Name = L"b3";
			this->b3->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b3->Size = System::Drawing::Size(45, 83);
			this->b3->TabIndex = 3;
			this->b3->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b3->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// b2
			// 
			this->b2->Location = System::Drawing::Point(98, 9);
			this->b2->Maximum = 12;
			this->b2->Minimum = -12;
			this->b2->Name = L"b2";
			this->b2->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b2->Size = System::Drawing::Size(45, 83);
			this->b2->TabIndex = 2;
			this->b2->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b2->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// b1
			// 
			this->b1->Location = System::Drawing::Point(58, 9);
			this->b1->Maximum = 12;
			this->b1->Minimum = -12;
			this->b1->Name = L"b1";
			this->b1->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b1->Size = System::Drawing::Size(45, 83);
			this->b1->TabIndex = 1;
			this->b1->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b1->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// b0
			// 
			this->b0->Location = System::Drawing::Point(18, 9);
			this->b0->Maximum = 12;
			this->b0->Minimum = -12;
			this->b0->Name = L"b0";
			this->b0->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->b0->Size = System::Drawing::Size(45, 83);
			this->b0->TabIndex = 0;
			this->b0->TickStyle = System::Windows::Forms::TickStyle::Both;
			this->b0->ValueChanged += gcnew System::EventHandler(this, &PlayerForm::band_ValueChanged);
			// 
			// masterPanel
			// 
			this->masterPanel->BackColor = System::Drawing::Color::Black;
			this->masterPanel->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->masterPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->masterPanel->Controls->Add(this->rateLbl);
			this->masterPanel->Controls->Add(this->tempoLbl);
			this->masterPanel->Controls->Add(this->pitchLbl);
			this->masterPanel->Controls->Add(this->label13);
			this->masterPanel->Controls->Add(this->rateValBar);
			this->masterPanel->Controls->Add(this->tempoValBar);
			this->masterPanel->Controls->Add(this->pitchValBar);
			this->masterPanel->Controls->Add(this->rightVolLbl);
			this->masterPanel->Controls->Add(this->leftVolLbl);
			this->masterPanel->Controls->Add(this->label12);
			this->masterPanel->Controls->Add(this->rightVolBar);
			this->masterPanel->Controls->Add(this->leftVolBar);
			this->masterPanel->Controls->Add(this->masterVolLbl);
			this->masterPanel->Controls->Add(this->label11);
			this->masterPanel->Controls->Add(this->masterVolBar);
			this->masterPanel->Location = System::Drawing::Point(470, 189);
			this->masterPanel->Name = L"masterPanel";
			this->masterPanel->Size = System::Drawing::Size(327, 242);
			this->masterPanel->TabIndex = 6;
			// 
			// rateLbl
			// 
			this->rateLbl->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->rateLbl->BackColor = System::Drawing::Color::Transparent;
			this->rateLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->rateLbl->ForeColor = System::Drawing::Color::White;
			this->rateLbl->Location = System::Drawing::Point(276, 24);
			this->rateLbl->Name = L"rateLbl";
			this->rateLbl->Size = System::Drawing::Size(30, 14);
			this->rateLbl->TabIndex = 29;
			this->rateLbl->Text = L"100";
			this->rateLbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// tempoLbl
			// 
			this->tempoLbl->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->tempoLbl->BackColor = System::Drawing::Color::Transparent;
			this->tempoLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->tempoLbl->ForeColor = System::Drawing::Color::White;
			this->tempoLbl->Location = System::Drawing::Point(234, 24);
			this->tempoLbl->Name = L"tempoLbl";
			this->tempoLbl->Size = System::Drawing::Size(30, 14);
			this->tempoLbl->TabIndex = 28;
			this->tempoLbl->Text = L"100";
			this->tempoLbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// pitchLbl
			// 
			this->pitchLbl->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->pitchLbl->BackColor = System::Drawing::Color::Transparent;
			this->pitchLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->pitchLbl->ForeColor = System::Drawing::Color::White;
			this->pitchLbl->Location = System::Drawing::Point(194, 24);
			this->pitchLbl->Name = L"pitchLbl";
			this->pitchLbl->Size = System::Drawing::Size(30, 14);
			this->pitchLbl->TabIndex = 27;
			this->pitchLbl->Text = L"100";
			this->pitchLbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->BackColor = System::Drawing::Color::Transparent;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label13->ForeColor = System::Drawing::Color::LightBlue;
			this->label13->Location = System::Drawing::Point(193, 4);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(117, 15);
			this->label13->TabIndex = 26;
			this->label13->Text = L"Pitch   Tempo   Rate";
			// 
			// rateValBar
			// 
			this->rateValBar->BackColor = System::Drawing::Color::Black;
			this->rateValBar->Location = System::Drawing::Point(280, 40);
			this->rateValBar->Maximum = 200;
			this->rateValBar->Name = L"rateValBar";
			this->rateValBar->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->rateValBar->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->rateValBar->RightToLeftLayout = true;
			this->rateValBar->Size = System::Drawing::Size(45, 200);
			this->rateValBar->TabIndex = 25;
			this->rateValBar->Value = 100;
			this->rateValBar->Scroll += gcnew System::EventHandler(this, &PlayerForm::rateValBar_Scroll);
			// 
			// tempoValBar
			// 
			this->tempoValBar->BackColor = System::Drawing::Color::Black;
			this->tempoValBar->Location = System::Drawing::Point(240, 40);
			this->tempoValBar->Maximum = 200;
			this->tempoValBar->Name = L"tempoValBar";
			this->tempoValBar->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->tempoValBar->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->tempoValBar->RightToLeftLayout = true;
			this->tempoValBar->Size = System::Drawing::Size(45, 200);
			this->tempoValBar->TabIndex = 24;
			this->tempoValBar->Value = 100;
			this->tempoValBar->Scroll += gcnew System::EventHandler(this, &PlayerForm::tempoValBar_Scroll);
			// 
			// pitchValBar
			// 
			this->pitchValBar->BackColor = System::Drawing::Color::Black;
			this->pitchValBar->Location = System::Drawing::Point(200, 40);
			this->pitchValBar->Maximum = 200;
			this->pitchValBar->Name = L"pitchValBar";
			this->pitchValBar->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->pitchValBar->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->pitchValBar->RightToLeftLayout = true;
			this->pitchValBar->Size = System::Drawing::Size(45, 200);
			this->pitchValBar->TabIndex = 23;
			this->pitchValBar->Value = 100;
			this->pitchValBar->Scroll += gcnew System::EventHandler(this, &PlayerForm::pitchValBar_Scroll);
			// 
			// rightVolLbl
			// 
			this->rightVolLbl->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->rightVolLbl->BackColor = System::Drawing::Color::Transparent;
			this->rightVolLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->rightVolLbl->ForeColor = System::Drawing::Color::White;
			this->rightVolLbl->Location = System::Drawing::Point(140, 24);
			this->rightVolLbl->Name = L"rightVolLbl";
			this->rightVolLbl->Size = System::Drawing::Size(30, 14);
			this->rightVolLbl->TabIndex = 22;
			this->rightVolLbl->Text = L"100";
			this->rightVolLbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// leftVolLbl
			// 
			this->leftVolLbl->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->leftVolLbl->BackColor = System::Drawing::Color::Transparent;
			this->leftVolLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->leftVolLbl->ForeColor = System::Drawing::Color::White;
			this->leftVolLbl->Location = System::Drawing::Point(87, 24);
			this->leftVolLbl->Name = L"leftVolLbl";
			this->leftVolLbl->Size = System::Drawing::Size(30, 14);
			this->leftVolLbl->TabIndex = 21;
			this->leftVolLbl->Text = L"100";
			this->leftVolLbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->BackColor = System::Drawing::Color::Transparent;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label12->ForeColor = System::Drawing::Color::LightBlue;
			this->label12->Location = System::Drawing::Point(95, 4);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(73, 15);
			this->label12->TabIndex = 20;
			this->label12->Text = L"L    VOL    R ";
			// 
			// rightVolBar
			// 
			this->rightVolBar->BackColor = System::Drawing::Color::Black;
			this->rightVolBar->Location = System::Drawing::Point(122, 40);
			this->rightVolBar->Maximum = 100;
			this->rightVolBar->Name = L"rightVolBar";
			this->rightVolBar->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->rightVolBar->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->rightVolBar->RightToLeftLayout = true;
			this->rightVolBar->Size = System::Drawing::Size(45, 200);
			this->rightVolBar->TabIndex = 19;
			this->rightVolBar->Scroll += gcnew System::EventHandler(this, &PlayerForm::leftRightVolBar_Scroll);
			// 
			// leftVolBar
			// 
			this->leftVolBar->BackColor = System::Drawing::Color::Black;
			this->leftVolBar->Location = System::Drawing::Point(93, 40);
			this->leftVolBar->Maximum = 100;
			this->leftVolBar->Name = L"leftVolBar";
			this->leftVolBar->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->leftVolBar->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->leftVolBar->Size = System::Drawing::Size(45, 200);
			this->leftVolBar->TabIndex = 18;
			this->leftVolBar->Scroll += gcnew System::EventHandler(this, &PlayerForm::leftRightVolBar_Scroll);
			// 
			// masterVolLbl
			// 
			this->masterVolLbl->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->masterVolLbl->BackColor = System::Drawing::Color::Transparent;
			this->masterVolLbl->Font = (gcnew System::Drawing::Font(L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->masterVolLbl->ForeColor = System::Drawing::Color::White;
			this->masterVolLbl->Location = System::Drawing::Point(14, 24);
			this->masterVolLbl->Name = L"masterVolLbl";
			this->masterVolLbl->Size = System::Drawing::Size(30, 14);
			this->masterVolLbl->TabIndex = 17;
			this->masterVolLbl->Text = L"100";
			this->masterVolLbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->BackColor = System::Drawing::Color::Transparent;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(161)));
			this->label11->ForeColor = System::Drawing::Color::LightBlue;
			this->label11->Location = System::Drawing::Point(-2, 4);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(86, 15);
			this->label11->TabIndex = 16;
			this->label11->Text = L"MASTER VOL ";
			// 
			// masterVolBar
			// 
			this->masterVolBar->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->masterVolBar->Location = System::Drawing::Point(19, 40);
			this->masterVolBar->Maximum = 100;
			this->masterVolBar->Name = L"masterVolBar";
			this->masterVolBar->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->masterVolBar->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->masterVolBar->Size = System::Drawing::Size(45, 200);
			this->masterVolBar->TabIndex = 0;
			this->masterVolBar->Scroll += gcnew System::EventHandler(this, &PlayerForm::masterVolBar_Scroll);
			// 
			// prevTrackBtn
			// 
			this->prevTrackBtn->Location = System::Drawing::Point(185, 15);
			this->prevTrackBtn->Name = L"prevTrackBtn";
			this->prevTrackBtn->Size = System::Drawing::Size(75, 23);
			this->prevTrackBtn->TabIndex = 3;
			this->prevTrackBtn->Text = L"<<";
			this->prevTrackBtn->UseVisualStyleBackColor = true;
			this->prevTrackBtn->Click += gcnew System::EventHandler(this, &PlayerForm::prevTrackBtn_Click);
			// 
			// nextTrackBtn
			// 
			this->nextTrackBtn->Location = System::Drawing::Point(301, 15);
			this->nextTrackBtn->Name = L"nextTrackBtn";
			this->nextTrackBtn->Size = System::Drawing::Size(75, 23);
			this->nextTrackBtn->TabIndex = 4;
			this->nextTrackBtn->Text = L">>";
			this->nextTrackBtn->UseVisualStyleBackColor = true;
			this->nextTrackBtn->Click += gcnew System::EventHandler(this, &PlayerForm::nextTrackBtn_Click);
			// 
			// PlayerForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(879, 436);
			this->Controls->Add(this->masterPanel);
			this->Controls->Add(this->backPanel);
			this->Controls->Add(this->vuPanel1);
			this->Controls->Add(this->playBtnPanel);
			this->Controls->Add(this->pathLbl);
			this->Controls->Add(this->dirBtn);
			this->Controls->Add(this->trackList);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->MaximizeBox = false;
			this->Name = L"PlayerForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"My Player";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &PlayerForm::PlayerForm_FormClosed);
			this->playBtnPanel->ResumeLayout(false);
			this->playBtnPanel->PerformLayout();
			this->vuPanel1->ResumeLayout(false);
			this->vuPanel1->PerformLayout();
			this->backPanel->ResumeLayout(false);
			this->equPanel->ResumeLayout(false);
			this->equPanel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->b0))->EndInit();
			this->masterPanel->ResumeLayout(false);
			this->masterPanel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rateValBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->tempoValBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pitchValBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->rightVolBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->leftVolBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->masterVolBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		/// Folder path button click
	private: System::Void dirBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		folderBrowserDialog->ShowDialog();
		soundsDirPath = folderBrowserDialog->SelectedPath->ToString();
		print(soundsDirPath);
		loadTracks();
	}

			 /// Close programm event
	private: System::Void PlayerForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
	{
		//player.~ZPlay();
		updateTimer->Stop();
		resourceWrite();
		print("Programm closed");
	}

			 /// Track list selected event
	private: System::Void trackList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{

	}

			 /// Track list double click
	private: System::Void trackList_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		trackName = trackList->SelectedItem->ToString();
		playingTrack = soundsDirPath + "/" + trackName;
		trackListIndex = trackList->SelectedIndex;
		loadTrackFile(playingTrack);
		if (player != nullptr) { startPlayer(); }
	}

			 /// Update timer tick
	private: System::Void updateTimer_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		visCounter++;
		if (visCounter > 1.5)
		{
			statusLbl->Visible = !statusLbl->Visible;
			visCounter = 0;
		}
		getPosition();
		vuMeter();
	}

			 /// Play button click
	private: System::Void playStopBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (playingTrack == "") { return; }
		player->GetStatus(statusInfo);
		if (!statusInfo.fPlay || statusInfo.fPause) { startPlayer(); }
		else { stopPlayer(); }		
	}

			 /// Pause button click
	private: System::Void pauseBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		pausePlayer();
	}

			 /// Bar traxk position mous click event
	private: System::Void barPosBack_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		setTrackPosition();
	}

			 /// Equalizer button change event
	private: System::Void band_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		for each (Control^ control in equPanel->Controls)
		{
			if (control->Name->IndexOf("b") == 0)
			{
				int numBand = 0;
				int::TryParse(control->Name->Substring(1), numBand);
				TrackBar^ bar = dynamic_cast<TrackBar^> (control);
				equalizerChange(numBand, (bar->Value) * 1000);
			}

		}

	}

			 /// Equalizer enable check event
	private: System::Void equCheck_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (player != nullptr)
		{
			equalizerEnable(equCheck->Checked);
		}		
	}

			 /// Flat equalizer button click
	private: System::Void equFlatBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		equalizerFlat();
	}

			 /// Master volemu change 
	private: System::Void masterVolBar_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		setMasterVolume(masterVolBar->Value);
		//print("volume");
	}

			 /// Left Right volume change
	private: System::Void leftRightVolBar_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		setLeftRightVolume(leftVolBar->Value, rightVolBar->Value);		
	}

			 /// Pitch change 
	private: System::Void pitchValBar_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		setPTR('P', pitchValBar->Value);
	}

			 /// Tempo change
	private: System::Void tempoValBar_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		setPTR('T', tempoValBar->Value);
	}

			 /// Rate change
	private: System::Void rateValBar_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		setPTR('R', rateValBar->Value);
	}

			 /// Previous track click button
	private: System::Void prevTrackBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		previousTrack();
	}

			 /// Next track click button
	private: System::Void nextTrackBtn_Click(System::Object^  sender, System::EventArgs^  e)
	{
		nextTrack();
	}
	};



}


