#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <xaudio2.h>
#include <fstream>

// pull in the sound in waves
class Wave {
private:
	WAVEFORMATEX m_wf;
	XAUDIO2_BUFFER m_xa;
	BYTE* m_waveData;
	IXAudio2SourceVoice* m_Source;

public:
	Wave(const char* szFile = NULL) : m_waveData(NULL) {
		ZeroMemory(&m_wf, sizeof(m_wf));
		ZeroMemory(&m_xa, sizeof(m_xa));
		m_Source = NULL;
		Load(szFile);
	}

	Wave(const Wave& c) : m_waveData(NULL) {
		m_wf = c.m_wf;
		m_xa = c.m_xa;
		
		if (c.m_waveData) {
			m_waveData = new BYTE[m_xa.AudioBytes];
			memcpy(m_waveData, c.m_waveData, m_xa.AudioBytes);
			m_xa.pAudioData = m_waveData;
		}

		m_Source = NULL;
	}

	~Wave() {
		if (m_waveData)
			delete[] m_waveData;
		m_waveData = NULL;
	}

	const XAUDIO2_BUFFER* xaBuffer() const { return &m_xa; }
	const WAVEFORMATEX* wf() const { return &m_wf; }
	bool Load(const char* szFile);

	bool IsPlaying() {
		XAUDIO2_VOICE_STATE st;
		m_Source->GetState(&st);

		return st.BuffersQueued > 0;
	}

	void Play();
};

class SoundEngine {
public:
	IXAudio2* m_Engine;
	IXAudio2MasteringVoice* m_Master;

	void Initialize();
};

#endif // !SOUNDENGINE_H
