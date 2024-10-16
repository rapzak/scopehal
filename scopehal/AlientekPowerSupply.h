/***********************************************************************************************************************
*                                                                                                                      *
* libscopehal v0.1                                                                                                     *
*                                                                                                                      *
* Copyright (c) 2012-2023 Andrew D. Zonenberg and contributors                                                         *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

#ifndef AlientekPowerSupply_h
#define AlientekPowerSupply_h

/**
	@brief A Riden RD6006 power supply or other equivalent model
 */
class AlientekPowerSupply
	: public virtual SCPIPowerSupply
	, public virtual HIDInstrument
{
public:
	AlientekPowerSupply(SCPITransport* transport);
	virtual ~AlientekPowerSupply();

	//Device information
	virtual uint32_t GetInstrumentTypesForChannel(size_t i) const override;

	//Device capabilities
	virtual bool SupportsIndividualOutputSwitching() override;
	virtual bool SupportsVoltageCurrentControl(int chan) override;

	//Read sensors
	virtual double GetPowerVoltageActual(int chan) override;	//actual voltage after current limiting
	virtual double GetPowerVoltageNominal(int chan) override;	//set point
	virtual double GetPowerCurrentActual(int chan) override;	//actual current drawn by the load
	virtual double GetPowerCurrentNominal(int chan) override;	//current limit
	virtual bool GetPowerChannelActive(int chan) override;

	//Configuration
	virtual void SetPowerVoltage(int chan, double volts) override;
	virtual void SetPowerCurrent(int chan, double amps) override;
	virtual void SetPowerChannelActive(int chan, bool on) override;
	virtual bool IsPowerConstantCurrent(int chan) override;

protected:
	enum Function : uint8_t {
		DEVICE_INFO = 0x10,  	// 16
		FIRM_INFO = 0x11,  		// 17
		START_TRANS = 0x12,  	// 18
		DATA_TRANS = 0x13,  	// 19
		END_TRANS = 0x14,  		// 20
		DEV_UPGRADE = 0x15,  	// 21
		BASIC_INFO = 0x30,  	// 48
		BASIC_SET = 0x35,  		// 53
		SYSTEM_INFO = 0x40,  	// 64
		SYSTEM_SET = 0x45,  	// 69
		SCAN_OUT = 0x50,  		// 80
		SERIAL_OUT = 0x55,  	// 85
		DISCONNECT = 0x80,  	// 128
		NONE = 0xFF  			// 255
	};

	enum Operation : uint8_t {
		OUTPUT = 0x20,  // 32
		SETTING = 0x40, // 64
		READ = 0x80  	// 128
	};

	void SendReceiveReport(Function function, int sequence = -1, std::vector<uint8_t>* data = nullptr);
	uint16_t CalculateCRC(const uint8_t *buff, size_t len);

	uint8_t m_deviceAdress = 0xFB;

public:
	static std::string GetDriverNameInternal();
	POWER_INITPROC(AlientekPowerSupply);
};

#endif
