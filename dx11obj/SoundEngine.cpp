#include "stdafx.h"
#include "SoundEngine.h"
#include "Engine.h"
#include <windows.h>

void SoundEngine::Initialize() {
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// create the audio engine
	if (FAILED(XAudio2Create(&m_Engine))) {
		CoUninitialize();
		throw exception("Can't create audio engine");
	}

	if (FAILED(m_Engine->CreateMasteringVoice(&m_Master))) {
		m_Engine->Release();
		CoUninitialize();
		throw exception("Can create mastering voice");
	}
}
 
bool Wave::Load(const char* szFile) {
	if (szFile == NULL)
		return false;

	std::ifstream inFile(szFile, std::ios::binary | std::ios::in);
	if (inFile.bad())
		return false;

	DWORD dwChunkId = 0, dwFileSize = 0, dwChunkSize = 0, dwExtra = 0;

	// look for 'RIFF' chunk identifier
	inFile.seekg(0, std::ios::beg);
	inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
	
	if (dwChunkId != 'FFIR') {
		inFile.close();
		return false;
	}

	inFile.seekg(4, std::ios::beg); // get file size
	inFile.read(reinterpret_cast<char*>(&dwFileSize), sizeof(dwFileSize));

	if (dwFileSize <= 16){
		inFile.close();
		return false;
	}
	
	inFile.seekg(8, std::ios::beg); // get file format
	inFile.read(reinterpret_cast<char*>(&dwExtra), sizeof(dwExtra));

	if (dwExtra != 'EVAW') {
		inFile.close();
		return false;
	}

	// look for 'fmt ' chunk id
	bool bFilledFormat = false;
	for (unsigned int i = 12; i < dwFileSize;) {
		inFile.seekg(i, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		inFile.seekg(i + 4, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));

		if (dwChunkId == ' tmf') {
			inFile.seekg(i + 8, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(&m_wf), sizeof(m_wf));
			bFilledFormat = true;
			break;
		}
		dwChunkSize += 8; // add offsets of the chunk id, and chunk size data entries
		dwChunkSize += 1;
		dwChunkSize &= 0xfffffffe; // guarantees WORD padding alignment
		i += dwChunkSize;
	}

	if (!bFilledFormat) {
		inFile.close();
		return false;
	}

	// look for 'data' chunk id
	bool bFilledData = false;

	for (unsigned int i = 12; i < dwFileSize;) {
		inFile.seekg(i, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		inFile.seekg(i + 4, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
		
		if (dwChunkId == 'atad') {
			m_waveData = new BYTE[dwChunkSize];
			inFile.seekg(i + 8, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(m_waveData), dwChunkSize);
			m_xa.AudioBytes = dwChunkSize;
			m_xa.pAudioData = m_waveData;
			m_xa.PlayBegin = 0;
			m_xa.PlayLength = 0;
			bFilledData = true;
			break;
		}

		dwChunkSize += 8; // add offsets of the chunk id, and chunk size data entries
		dwChunkSize += 1;
		dwChunkSize &= 0xfffffffe; // guarantees WORD padding alignment
		i += dwChunkSize;
	}
	
	if (!bFilledData) {
		inFile.close();
		return false;
	}
	inFile.close();

	if (FAILED(Engine.m_Sound.m_Engine->CreateSourceVoice(&(m_Source), wf()))) {
		Engine.m_Sound.m_Engine->Release();
		CoUninitialize();
		throw exception("Create source voice failed");
	}

	m_Source->Start();

	return true;
}

void Wave::Play() {
	m_Source->SubmitSourceBuffer(xaBuffer());
}
