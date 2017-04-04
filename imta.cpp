/*
* =====================================================================================
*
*       Filename:  imta.cpp
*
*    Description:  imta
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  LK,WYB
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<cmath>
#include<memory.h>
#include<iostream>
#include"imta.h"
#include"function.h"

using namespace std;

const double imta::s_PI = 3.1415926535897932384626433832795f;
const double imta::s_PINeg = -3.1415926535897932384626433832795f;
const double imta::s_PI2 = 6.283185307179586476925286766559f;
const double imta::s_PI_HALF = 1.5707963267948966192313216916398f;
const double imta::s_DEGREE_TO_PI = 0.01745329251994329576923690768489f;
const double imta::s_SQRT_HALF = 0.70710678118654752440084436210485f;
const double imta::s_SQRT_THREE = 1.73205080756887729f;
const double imta::s_C = 299792458.0f;
const double imta::s_FC = 6e9f;


const double imta::s_ConstantUMiLoS[25] =
{
	0.753065949852806f, 0.241023875447849f, 0.454091158552085f, -0.097177920212920f, -0.398944655540474f,
	0.241023875447849f, 0.929354051080552f, 0.137998056490968f, -0.242351266621617f, -0.020759074542993f,
	0.454091158552085f, 0.137998056490968f, 0.861515602815864f, -0.175603398954279f, -0.041377149612582f,
	-0.097177920212920f, -0.242351266621617f, -0.175603398954279f, 0.915728740196339f, 0.249853229004786f,
	-0.398944655540474f, -0.020759074542993f, -0.041377149612582f, 0.249853229004786f, 0.881063855850205f
};
const double imta::s_ConstantUMiNLoS[25] =
{
	0.913514893522226f, 0.0f, 0.178007039816570f, -0.365792336017060f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.178007039816570f, 0.0f, 0.967794909860288f, -0.178007039816570f, 0.0f,
	-0.365792336017060f, 0.0f, -0.178007039816570f, 0.913514893522227f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double imta::s_ConstantUMiO2I[25] =
{
	0.896950683840360f, 0.241943793527554f, 0.223605545793171f, -0.294861376620174f, 0.0f,
	0.241943793527554f, 0.959179465107684f, -0.030708888757422f, 0.143160464655990f, 0.0f,
	0.223605545793171f, -0.030708888757422f, 0.973505191933743f, 0.036676495468768f, 0.0f,
	-0.294861376620174f, 0.143160464655990f, 0.036676495468768f, 0.944042734529146f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double imta::s_ConstantSMaLoS[25] =
{
	0.854017192528818f, -0.040746823503881f, 0.423846524961968f, -0.298912118384658f, 0.0f,
	-0.040746823503881f, 0.958761974312789f, -0.023404035164252f, -0.280298812206218f, 0.0f,
	0.423846524961968f, -0.023404035164252f, 0.879509345569223f, -0.215103894600008f, 0.0f,
	-0.298912118384658f, -0.280298812206218f, -0.215103894600008f, 0.886461750943879f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double imta::s_ConstantSMaNLoS[25] =
{
	0.888863320019977f, -0.028738390905236f, 0.394136210972923f, -0.231846394000744f, 0.0f,
	-0.028738390905236f, 0.976874642167262f, 0.011594978528380f, -0.211555181576079f, 0.0f,
	0.394136210972923f, 0.011594978528380f, 0.917601710925213f, 0.050292184084163f, 0.0f,
	-0.231846394000744f, -0.211555181576079f, 0.050292184084163f, 0.948136251262026f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double imta::s_ConstantUMaLoS[25] =
{
	0.806310951408682f, 0.245016774934418f, 0.479171304494613f, -0.120392914754038f, -0.213845356893992f,
	0.245016774934418f, 0.924083471541761f, -0.108566442433108f, -0.271617534928914f, 0.021766026753229f,
	0.479171304494613f, -0.108566442433108f, 0.825678402680475f, -0.271600920527001f, -0.055644441252067f,
	-0.120392914754038f, -0.271617534928914f, -0.271600920527001f, 0.915216117252956f, -0.018489442540902f,
	-0.213845356893992f, 0.021766026753229f, -0.055644441252067f, -0.018489442540902f, 0.974863190445988f
};
const double imta::s_ConstantUMaNLoS[25] =
{
	0.913941405256431f, 0.147728073775767f, 0.318005795482914f, -0.204352240055453f, 0.0f,
	0.147728073775767f, 0.913941405256431f, 0.204352240055453f, -0.318005795482914f, 0.0f,
	0.318005795482914f, 0.204352240055453f, 0.923123353576218f, 0.070397088759368f, 0.0f,
	-0.204352240055453f, -0.318005795482914f, 0.070397088759368f, 0.923123353576218f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double imta::s_ConstantRMaLoS[25] =
{
	0.965925826289068f, 0.0f, 0.0f, -0.258819045102521f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.258819045102521f, 0.0f, 0.0f, 0.965925826289068f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double imta::s_ConstantRMaNLoS[25] =
{
	0.955557150656242f, -0.173466133506044f, 0.0f, -0.238369529001059f, 0.0f,
	-0.173466133506044f, 0.938008596337461f, 0.0f, 0.300082278256296f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.238369529001059f, 0.300082278256296f, 0.0f, 0.923650688258459f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const double imta::s_AngleOffset[s_SubPathNum] =
{
	7.8016217564146532088488977351458e-4f, -7.8016217564146532088488977351458e-4f,
	0.00246615023306798769219317505587f, -0.00246615023306798769219317505587f,
	0.00434936049596986930569383739507f, -0.00434936049596986930569383739507f,
	0.00648389817115893437827151120494f, -0.00648389817115893437827151120494f,
	0.00895179373347891640004160995158f, -0.00895179373347891640004160995158f,
	0.01186300292580545813435032615342f, -0.01186300292580545813435032615342f,
	0.01543569190463785077831312115652f, -0.01543569190463785077831312115652f,
	0.02003812514214689787266089371302f, -0.02003812514214689787266089371302f,
	0.02652027798405383792135548122719f, -0.02652027798405383792135548122719f,
	0.03761359070972979671228245975171f, -0.03761359070972979671228245975171f
};
const double imta::s_MidPathDelayOffset[s_MidPathNum] =
{
	0.0f, 5.0e-9f, 10.0e-9f
};
const int imta::s_MidPathIndex[s_SubPathNum] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0
};


void imta::randomGaussian(double *t_pfArray, long t_ulNumber, double t_fMean, double t_fStandardDeviation) {
	long ulHalfNum = t_ulNumber / 2;
	if (ulHalfNum)
	{
		double *pfTemp1 = new double[ulHalfNum];
		double *pfTemp2 = new double[ulHalfNum];
		randomUniform(pfTemp1, ulHalfNum, 1.0f, 0.0f, true);
		randomUniform(pfTemp2, ulHalfNum, s_PI, s_PINeg, false);
		for (long ulTemp = 0; ulTemp != ulHalfNum; ++ulTemp)
		{
			t_pfArray[ulTemp * 2] = sqrt(log(pfTemp1[ulTemp]) * -2.0f) * cos(pfTemp2[ulTemp]) * t_fStandardDeviation + t_fMean;
			t_pfArray[ulTemp * 2 + 1] = sqrt(log(pfTemp1[ulTemp]) * -2.0f) * sin(pfTemp2[ulTemp]) * t_fStandardDeviation + t_fMean;
		}
		memory_clean::safe_delete(pfTemp1, true);
		memory_clean::safe_delete(pfTemp2, true);
	}
	if (t_ulNumber % 2)
	{
		double fTemp1;
		double fTemp2;
		randomUniform(&fTemp1, 1, 1.0f, 0.0f, true);
		randomUniform(&fTemp2, 1, s_PI, s_PINeg, false);
		t_pfArray[t_ulNumber - 1] = sqrt(log(fTemp1) * -2.0f) * cos(fTemp2) * t_fStandardDeviation + t_fMean;
	}

	return;
}


void imta::randomUniform(double *t_pfArray, long t_ulNumber, double t_fUpBound, double t_fDownBound, bool t_bFlagZero)
{
	for (long ulTemp = 0; ulTemp != t_ulNumber; ++ulTemp)
	{
		do
		{
			t_pfArray[ulTemp] = (t_fUpBound - t_fDownBound) * rand() / RAND_MAX + t_fDownBound;
		} while (t_bFlagZero && (t_pfArray[ulTemp] == 0.0f));
	}

	return;
}


void imta::sortBubble(double *t_pfArray, int t_wNumber, bool t_bFlagDirection, bool t_bFlagFabs)
{
	double fTemp;
	bool bFlagDone;
	for (int i1 = 0; i1 != t_wNumber - 1; ++i1)
	{
		bFlagDone = true;
		for (int i2 = 0; i2 != t_wNumber - 1 - i1; ++i2)
		{
			if (t_bFlagDirection)
			{
				if (t_bFlagFabs)
				{
					if (fabs(t_pfArray[i2]) < fabs(t_pfArray[i2 + 1]))
					{
						fTemp = t_pfArray[i2];
						t_pfArray[i2] = t_pfArray[i2 + 1];
						t_pfArray[i2 + 1] = fTemp;
						bFlagDone = false;
					}
				}
				else
				{
					if (t_pfArray[i2] < t_pfArray[i2 + 1])
					{
						fTemp = t_pfArray[i2];
						t_pfArray[i2] = t_pfArray[i2 + 1];
						t_pfArray[i2 + 1] = fTemp;
						bFlagDone = false;
					}
				}
			}
			else
			{
				if (t_bFlagFabs)
				{
					if (fabs(t_pfArray[i2]) > fabs(t_pfArray[i2 + 1]))
					{
						fTemp = t_pfArray[i2];
						t_pfArray[i2] = t_pfArray[i2 + 1];
						t_pfArray[i2 + 1] = fTemp;
						bFlagDone = false;
					}
				}
				else
				{
					if (t_pfArray[i2] > t_pfArray[i2 + 1])
					{
						fTemp = t_pfArray[i2];
						t_pfArray[i2] = t_pfArray[i2 + 1];
						t_pfArray[i2 + 1] = fTemp;
						bFlagDone = false;
					}
				}
			}
		}
		if (bFlagDone)
		{
			break;
		}
	}

	return;
}


void imta::selectMax(double *t_pfArray, int t_byNumber, int *t_pbyFirst, int *t_pbySecond)
{
	int byFisrtIndex;
	int bySecondIndex;
	if (t_pfArray[0] < t_pfArray[1])
	{
		byFisrtIndex = 1;
		bySecondIndex = 0;
	}
	else
	{
		byFisrtIndex = 0;
		bySecondIndex = 1;
	}
	for (int byTemp = 2; byTemp != t_byNumber; ++byTemp)
	{
		if (t_pfArray[byFisrtIndex] < t_pfArray[byTemp])
		{
			bySecondIndex = byFisrtIndex;
			byFisrtIndex = byTemp;
		}
		else
		{
			if (t_pfArray[bySecondIndex] < t_pfArray[byTemp])
			{
				bySecondIndex = byTemp;
			}
		}
	}
	*t_pbyFirst = byFisrtIndex;
	*t_pbySecond = bySecondIndex;

	return;
}


imta::imta() {
	m_Gain = nullptr;
	m_SinAoD = nullptr;
	m_CosAoD = nullptr;
	m_SinAoA = nullptr;
	m_CosAoA = nullptr;
	m_Phase = nullptr;
	m_GainLoS = 0;
	m_SinAoDLoS = 0;
	m_PhaseLoS = nullptr;
	m_AntGain = 0;
	m_MaxAttenu = 0; // dBm
	m_TxAntNum = 0;
	m_RxAntNum = 0;
	m_LoS = false;
	m_Built = false;
	m_Enable = false;
	m_PLSF = 0.0f;
	m_TxAntSpacing = nullptr;
	m_TxSlantAngle = nullptr;
	m_RxAntSpacing = nullptr;
	m_RxSlantAngle = nullptr;
	//FFT参数初始化
	m_FFTIndex = nullptr;
}

imta::~imta() {
	refresh();
	memory_clean::safe_delete(m_TxAntSpacing, true);
	memory_clean::safe_delete(m_RxAntSpacing, true);
	memory_clean::safe_delete(m_TxSlantAngle, true);
	memory_clean::safe_delete(m_RxSlantAngle, true);
}

bool imta::build(double* t_Pl, double t_fFrequency/*Hz*/, location &t_eLocation, antenna &t_eAntenna, double t_fVelocityi/*km/h*/, double t_fVelocityj/*km/h*/, double t_fVAnglei/*degree*/, double t_fVAnglej/*degree*/) {
	m_Built = false;
	m_AntGain = t_eAntenna.antGain * 0.1f;
	m_TxAntNum = t_eAntenna.byTxAntNum;
	m_RxAntNum = t_eAntenna.byRxAntNum;

	
	memory_clean::safe_delete(m_TxAntSpacing, true);
	m_TxAntSpacing = new double[m_TxAntNum];

	memory_clean::safe_delete(m_TxSlantAngle);
	m_TxSlantAngle = new double[m_TxAntNum];

	memory_clean::safe_delete(m_RxAntSpacing, true);
	m_RxAntSpacing = new double[m_RxAntNum];

	memory_clean::safe_delete(m_RxSlantAngle, true);
	m_RxSlantAngle = new double[m_RxAntNum];

	for (int byTempTxAnt = 0; byTempTxAnt != m_TxAntNum; ++byTempTxAnt) {
		m_TxAntSpacing[byTempTxAnt] = t_eAntenna.TxAntSpacing[byTempTxAnt] * s_PI2;
		m_TxSlantAngle[byTempTxAnt] = t_eAntenna.TxSlantAngle[byTempTxAnt] * s_DEGREE_TO_PI;
	}
	for (int byTempRxAnt = 0; byTempRxAnt != m_RxAntNum; ++byTempRxAnt) {
		m_RxAntSpacing[byTempRxAnt] = t_eAntenna.RxAntSpacing[byTempRxAnt] * s_PI2;
		m_RxSlantAngle[byTempRxAnt] = t_eAntenna.RxSlantAngle[byTempRxAnt] * s_DEGREE_TO_PI;
	}
	m_TxAngle = t_eAntenna.TxAngle * s_DEGREE_TO_PI;
	m_RxAngle = t_eAntenna.RxAngle * s_DEGREE_TO_PI;
	m_Velocityi = t_fVelocityi / 3.6f * t_fFrequency * s_PI2 / s_C;
	m_Velocityj = t_fVelocityj / 3.6f * t_fFrequency * s_PI2 / s_C;
	m_VAnglei = t_fVAnglei * s_DEGREE_TO_PI;
	m_VAnglej = t_fVAnglej * s_DEGREE_TO_PI;
	//FFT相关
	m_FFTNum = 1;
	m_FFTOrder = 0;
	m_HNum = 666;
	while (true)
	{
		if (m_HNum > m_FFTNum)
		{
			m_FFTNum <<= 1;
			++m_FFTOrder;
		}
		else
		{
			m_FFTTime = 1.0f / 15000 / m_FFTNum;
			break;
		}
	}

	double fTemp;
	double fSFSTD;
	double fPL1;
	double fPL2;
	double fDistanceBP = 4 * (t_eLocation.VeUEAntH - 1)*(t_eLocation.VeUEAntH - 1)*t_fFrequency / s_C;

	switch (t_eLocation.locationType)
	{
	case Los:
		if (3 < t_eLocation.distance&&t_eLocation.distance < fDistanceBP)
		{
			m_PLSF = 22.7f * log10(t_eLocation.distance) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);//转换为GHz
		}
		else
		{
			if (fDistanceBP < t_eLocation.distance&&t_eLocation.distance < 5000)
			{
				m_PLSF = 40.0f * log10(t_eLocation.distance) + 7.56f - 17.3f * log10(t_eLocation.VeUEAntH - 1) - 17.3 * log10(t_eLocation.VeUEAntH - 1) + 2.7f *(log10(t_fFrequency) - 9.0f);
			}
			else if (t_eLocation.distance < 3)
			{
				m_PLSF = 22.7f * log10(3) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
			}
		}
		break;
	case Nlos:
		if (t_eLocation.manhattan) {
			fTemp = (2.8f - 0.0024f * t_eLocation.distance1) > 1.84f ? (2.8f - 0.0024f * t_eLocation.distance1) : 1.84f;
			if (3 < t_eLocation.distance1&&t_eLocation.distance1 < fDistanceBP)
			{
				fPL1 = 22.7f * log10(t_eLocation.distance1) + 27.0f + 20.0f *(log10(t_fFrequency) - 9.0f);
			}
			else
			{
				if (fDistanceBP < t_eLocation.distance1&&t_eLocation.distance1 < 5000)
				{
					fPL1 = 40.0f * log10(t_eLocation.distance1) + 7.56f - 17.3f * log10(t_eLocation.VeUEAntH - 1) - 17.3 * log10(t_eLocation.RSUAntH - 1) + 2.7f * (log10(t_fFrequency) - 9.0f);
				}
				else if (t_eLocation.distance1 < 3)
				{
					fPL1 = 22.7f * log10(3) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
				}
			}
			fPL1 = fPL1 + 17.3f - 12.5f*fTemp + 10 * fTemp * log10(t_eLocation.distance2) + 3 * (log10(t_fFrequency) - 9.0f);
			fTemp = (2.8f - 0.0024f * t_eLocation.distance2) > 1.84f ? (2.8f - 0.0024f * t_eLocation.distance2) : 1.84f;
			if (3 < t_eLocation.distance2&&t_eLocation.distance2 < fDistanceBP)
			{
				fPL2 = 22.7f * log10(t_eLocation.distance2) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
			}
			else
			{
				if (fDistanceBP < t_eLocation.distance2&&t_eLocation.distance2 < 5000)
				{
					fPL2 = 40.0f * log10(t_eLocation.distance2) + 7.56f - 17.3f * log10(t_eLocation.VeUEAntH - 1) - 17.3 * log10(t_eLocation.RSUAntH - 1) + 2.7f * (log10(t_fFrequency) - 9.0f);
				}
				else if (t_eLocation.distance2 < 3)
				{
					fPL2 = 22.7f * log10(3) + 27.0f + 20.0f *(log10(t_fFrequency) - 9.0f);
				}
			}
			fPL2 = fPL2 + 17.3f - 12.5f*fTemp + 10 * fTemp * log10(t_eLocation.distance1) + 3 * (log10(t_fFrequency) - 9.0f);
			m_PLSF = fPL1 < fPL2 ? fPL1 : fPL2;
		}
		else {
			//m_PLSF = (44.9 - 6.55*log10(t_eLocation.VeUEAntH))*log10(t_eLocation.distance) + 5.83*log10(t_eLocation.VeUEAntH) + 18.38 + 23 * (log10(t_fFrequency)-9.0f);
			m_PLSF = 200;
		}
		break;
	}

	fSFSTD = 3.0f;
	double fDSMean;
	double fDSSTD;
	double fASDMean;
	double fASDSTD;
	double fASAMean;
	double fASASTD;
	double fKMean;
	double fKSTD;
	const double *cpfConstant;

	{
		fDSMean = -6.89f;
		fDSSTD = 0.54f;
		fASDMean = 1.41f;
		fASDSTD = 0.17f;
		fASAMean = 1.84f;
		fASASTD = 0.15f;
		fKMean = 0.0f;
		fKSTD = 0.0f;
		cpfConstant = s_ConstantUMiNLoS;
		m_DSRatio = 3.0f;
		m_XPR = 8.0f;
		m_PathNum = 19;
		m_AoDRatio = 10.0f;
		m_AoARatio = 22.0f;
		m_PathShadowSTD = 3.0f;
		m_C = 1.273f;
	}

	m_XPR = pow(10.0f, m_XPR * -0.05f);
	m_PathShadowSTD /= 10.0f;

	double afTemp[5] = { 0.0f };
	for (int byTemp = 0; byTemp != 5; ++byTemp)
	{
		for (int byTempTime = 0; byTempTime != 5; ++byTempTime)
		{
			afTemp[byTemp] += (cpfConstant[byTemp * 5 + byTempTime] * t_eLocation.posCor[byTempTime]);
		}
	}
	m_DS = pow(10.0f, fDSSTD * afTemp[0] + fDSMean);
	m_AoD = pow(10.0f, fASDSTD * afTemp[1] + fASDMean);
	m_AoD = m_AoD < 104.0f ? m_AoD : 104.0f;
	m_AoA = pow(10.0f, fASASTD * afTemp[2] + fASAMean);
	m_AoA = m_AoA < 104.0f ? m_AoA : 104.0f;
	m_AoD *= s_DEGREE_TO_PI;
	m_AoA *= s_DEGREE_TO_PI;
	m_PLSF += fSFSTD * afTemp[3];
	*t_Pl = pow(10, -m_PLSF / 10);
	m_KDB = fKSTD * afTemp[4] + fKMean;
	m_K = pow(10.0f, m_KDB * 0.1f);
	m_DS *= -m_DSRatio;

	m_Built = true;

	return true;
}

bool imta::enable(bool *t_pbEnable)
{
	refresh();
    
	m_Gain = new double[m_PathNum * s_SubPathNum];
	m_SinAoD = new double[m_PathNum * s_SubPathNum];
	m_CosAoD = new double[m_PathNum * s_SubPathNum];
	m_Phase = new double[m_TxAntNum * m_RxAntNum * m_PathNum * s_SubPathNum * 2];	
	m_SinAoA = new double[m_PathNum * s_SubPathNum];
	m_CosAoA = new double[m_PathNum * s_SubPathNum];
	m_FFTIndex = new int[m_PathNum];

	double *pfPathDelay = new double[m_PathNum];
	double *pfPathPower = new double[m_PathNum];
	double fPowerTotal = 0.0f;
	double *pfAoD = new double[m_PathNum];
	double *pfAoA = new double[m_PathNum];
	double *pfXAoD = new double[m_PathNum];
	double *pfXAoA = new double[m_PathNum];
	double fPowerMax;
	int byStoreIndex = 0;
	randomUniform(pfPathDelay, m_PathNum, 1.0f, 0.0f, true);
	for (int byTempPath = 0; byTempPath != m_PathNum; ++ byTempPath)
	{
		pfPathDelay[byTempPath] = m_DS * log(pfPathDelay[byTempPath]);
	}
	sortBubble(pfPathDelay, m_PathNum, false, false); //从小到大
	for (int byTempPath = 0; byTempPath != m_PathNum; ++ byTempPath)
	{
		pfPathDelay[m_PathNum - 1 - byTempPath] -= pfPathDelay[0];
	}

	randomGaussian(pfPathPower, m_PathNum, 0.0f, m_PathShadowSTD);
	for (int byTempPath = 0; byTempPath != m_PathNum; ++ byTempPath)
	{
		pfPathPower[byTempPath] = exp((m_DSRatio - 1.0f) * pfPathDelay[byTempPath] / m_DS) * pow(10.0f, pfPathPower[byTempPath]);
		fPowerTotal += pfPathPower[byTempPath];
	}
	for (int byTempPath = 0; byTempPath != m_PathNum; ++ byTempPath)
	{
		pfPathPower[byTempPath] /= fPowerTotal;
	}

	selectMax(pfPathPower, static_cast<int>(m_PathNum), &m_PathFirst, &m_PathSecond);

	fPowerMax = pfPathPower[m_PathFirst];
	    
	randomUniform(pfXAoD, m_PathNum, 1.0f, -1.0f, true);
	randomUniform(pfXAoA, m_PathNum, 1.0f, -1.0f, true);
	randomGaussian(pfAoD, m_PathNum, 0.0f, m_AoD / 7.0f);//在winner 2 是除以5.0f
	randomGaussian(pfAoA, m_PathNum, 0.0f, m_AoA / 7.0f);
		

	for (int byTempPath = 0; byTempPath != m_PathNum; ++ byTempPath)
		{
			if (pfXAoD[byTempPath] > 0.0f)
			{
				pfAoD[byTempPath] += (2.0f * m_AoD * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_C);
			}
			else
			{
				pfAoD[byTempPath] -= (2.0f * m_AoD * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_C);
			}
			if (pfXAoA[byTempPath] > 0.0f)
			{
				pfAoA[byTempPath] += (2.0f * m_AoA * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_C);
			}
			else
			{
				pfAoA[byTempPath] -= (2.0f * m_AoA * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_C);
			}
			pfAoD[byTempPath] += m_TxAngle;
            pfAoA[byTempPath] += m_RxAngle;

		}

	for (int byTempPath = 0; byTempPath != m_PathNum; ++byTempPath)
	{
		pfPathPower[byTempPath] /= s_SubPathNum;
		m_FFTIndex[byStoreIndex] = (int)floor(pfPathDelay[byTempPath] / m_FFTTime + 0.5f);
		if (m_FFTIndex[byStoreIndex] >= m_FFTNum)
		{
			m_FFTIndex[byStoreIndex] = m_FFTNum - 1;
		}
		++byStoreIndex;
	}

		for (int byTempPath = 0; byTempPath != m_PathNum; ++ byTempPath)
 		{
      		for (int byTempSubPath = 0; byTempSubPath != s_SubPathNum; ++ byTempSubPath)
		    {
				m_Gain[byTempPath * s_SubPathNum + byTempSubPath] = 0.6f;//m_fAntGain
				m_Gain[byTempPath * s_SubPathNum + byTempSubPath] = pow(10.0f, m_Gain[byTempPath * s_SubPathNum + byTempSubPath]);
				m_Gain[byTempPath * s_SubPathNum + byTempSubPath] *= pfPathPower[byTempPath];
				m_Gain[byTempPath * s_SubPathNum + byTempSubPath] = sqrt(m_Gain[byTempPath * s_SubPathNum + byTempSubPath]);
			    m_SinAoD[byTempPath * s_SubPathNum + byTempSubPath] = pfAoD[byTempPath] + s_AngleOffset[byTempSubPath] * m_AoDRatio;
				m_SinAoD[byTempPath * s_SubPathNum + byTempSubPath] = sin(m_SinAoD[byTempPath * s_SubPathNum + byTempSubPath]);
			   	m_SinAoA[byTempPath * s_SubPathNum + byTempSubPath] = pfAoA[byTempPath] + s_AngleOffset[byTempSubPath] * m_AoARatio;
				m_CosAoA[byTempPath * s_SubPathNum + byTempSubPath] = cos(m_SinAoA[byTempPath * s_SubPathNum + byTempSubPath] - m_VAnglei) * m_Velocityi + cos(m_SinAoA[byTempPath * s_SubPathNum + byTempSubPath] - m_VAnglej) * m_Velocityj;		     	
				m_SinAoA[byTempPath * s_SubPathNum + byTempSubPath] = sin(m_SinAoA[byTempPath * s_SubPathNum + byTempSubPath]);
				
			}
	}

	double *pfPhasePol = new double[m_PathNum * s_SubPathNum * 4];
	double *pfSlantVV = new double[m_TxAntNum * m_RxAntNum];
	double *pfSlantVH = new double[m_TxAntNum * m_RxAntNum];
	double *pfSlantHV = new double[m_TxAntNum * m_RxAntNum];
	double *pfSlantHH = new double[m_TxAntNum * m_RxAntNum];
	for (int byTempTxAnt = 0; byTempTxAnt != m_TxAntNum; ++ byTempTxAnt)
	{
		for (int byTempRxAnt = 0; byTempRxAnt != m_RxAntNum; ++ byTempRxAnt)
		{
			pfSlantVV[byTempTxAnt * m_RxAntNum + byTempRxAnt] = sin(m_TxSlantAngle[byTempTxAnt]) * sin(m_RxSlantAngle[byTempRxAnt]);
			pfSlantVH[byTempTxAnt * m_RxAntNum + byTempRxAnt] = sin(m_TxSlantAngle[byTempTxAnt]) * cos(m_RxSlantAngle[byTempRxAnt]) * m_XPR;
			pfSlantHV[byTempTxAnt * m_RxAntNum + byTempRxAnt] = cos(m_TxSlantAngle[byTempTxAnt]) * sin(m_RxSlantAngle[byTempRxAnt]) * m_XPR;
			pfSlantHH[byTempTxAnt * m_RxAntNum + byTempRxAnt] = cos(m_TxSlantAngle[byTempTxAnt]) * cos(m_RxSlantAngle[byTempRxAnt]);
		}
	}

	randomUniform(pfPhasePol, m_PathNum * s_SubPathNum * 4, s_PI, s_PINeg, false);
	for (int byTempTxAnt = 0; byTempTxAnt != m_TxAntNum; ++ byTempTxAnt)
	{
		for (int byTempRxAnt = 0; byTempRxAnt != m_RxAntNum; ++ byTempRxAnt)
		{
				for (int byTempPath = 0; byTempPath != m_PathNum; ++ byTempPath)
				{
					for (int byTempSubPath = 0; byTempSubPath != s_SubPathNum; ++ byTempSubPath)
					{
						m_Phase[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum * 2 + byTempRxAnt * m_PathNum * s_SubPathNum * 2 + byTempPath * s_SubPathNum * 2 + byTempSubPath * 2] =
							pfSlantVV[byTempTxAnt * m_RxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * s_SubPathNum * 4 + byTempSubPath * 4]) +
							pfSlantVH[byTempTxAnt * m_RxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * s_SubPathNum * 4 + byTempSubPath * 4 + 1]) +
							pfSlantHV[byTempTxAnt * m_RxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * s_SubPathNum * 4 + byTempSubPath * 4 + 2]) +
							pfSlantHH[byTempTxAnt * m_RxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * s_SubPathNum * 4 + byTempSubPath * 4 + 3]);
						m_Phase[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum * 2 + byTempRxAnt * m_PathNum * s_SubPathNum * 2 + byTempPath * s_SubPathNum * 2 + byTempSubPath * 2 + 1] =
							pfSlantVV[byTempTxAnt * m_RxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * s_SubPathNum * 4 + byTempSubPath * 4]) +
							pfSlantVH[byTempTxAnt * m_RxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * s_SubPathNum * 4 + byTempSubPath * 4 + 1]) +
							pfSlantHV[byTempTxAnt * m_RxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * s_SubPathNum * 4 + byTempSubPath * 4 + 2]) +
							pfSlantHH[byTempTxAnt * m_RxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * s_SubPathNum * 4 + byTempSubPath * 4 + 3]);
					}
				}
			}
		}

	memory_clean::safe_delete(pfPhasePol, true);
	memory_clean::safe_delete(pfSlantVV, true);
	memory_clean::safe_delete(pfSlantVH, true);
	memory_clean::safe_delete(pfSlantHV, true);
	memory_clean::safe_delete(pfSlantHH, true);
	memory_clean::safe_delete(pfPathDelay, true);
	memory_clean::safe_delete(pfPathPower, true);
	memory_clean::safe_delete(pfXAoD, true);
	memory_clean::safe_delete(pfXAoA, true);
	memory_clean::safe_delete(pfAoD, true);
	memory_clean::safe_delete(pfAoA, true);
	return true;
}

void imta::calculate(double* t_HAfterFFT, double t_fT/*s */, double *t_pfTemp, double *t_pfSin, double *t_pfCos,double *t_pfH,double *t_pfHFFT)
{

	memset(t_pfH, 0, m_TxAntNum * m_RxAntNum * m_PathNum * 2 * sizeof(double));

		for (int byTempTxAnt = 0; byTempTxAnt != m_TxAntNum; ++ byTempTxAnt)
		{
			for (int byTempRxAnt = 0; byTempRxAnt != m_RxAntNum; ++ byTempRxAnt)
			{
				for (int byTempPath = 0; byTempPath != m_PathNum; ++ byTempPath)
				{
						for (int byTempSubPath = 0; byTempSubPath != s_SubPathNum; ++ byTempSubPath)
						{
							t_pfTemp[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath] =
						            m_TxAntSpacing[byTempTxAnt] * m_SinAoD[byTempPath * s_SubPathNum + byTempSubPath] +
							         m_RxAntSpacing[byTempRxAnt] * m_SinAoA[byTempPath * s_SubPathNum + byTempSubPath] +
							         m_CosAoA[byTempPath * s_SubPathNum + byTempSubPath] * t_fT;//t_fT为时间
						
							t_pfCos[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath] =
								   cos(t_pfTemp[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath]*2*s_PI*s_C/s_FC);
							
							t_pfSin[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath] =
							       sin(t_pfTemp[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath]*2*s_PI*s_C/s_FC);

							t_pfH[byTempTxAnt * m_RxAntNum * m_PathNum  * 2 + byTempRxAnt * m_PathNum  * 2 + byTempPath * 2 ] +=
								m_Gain[byTempPath * s_SubPathNum + byTempSubPath] *
								(t_pfCos[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath] *
								m_Phase[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum * 2 + byTempRxAnt * m_PathNum * s_SubPathNum * 2 + byTempPath * s_SubPathNum * 2 + byTempSubPath * 2] -
								t_pfSin[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath] *
								m_Phase[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum * 2 + byTempRxAnt * m_PathNum * s_SubPathNum * 2 + byTempPath * s_SubPathNum * 2 + byTempSubPath * 2+ 1]);
							t_pfH[byTempTxAnt * m_RxAntNum * m_PathNum  * 2 + byTempRxAnt * m_PathNum  * 2 + byTempPath * 2 + 1] +=
								m_Gain[byTempPath * s_SubPathNum + byTempSubPath] *
								(t_pfSin[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath] *
								m_Phase[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum * 2 + byTempRxAnt * m_PathNum * s_SubPathNum * 2 + byTempPath * s_SubPathNum * 2 + byTempSubPath * 2] +
								t_pfCos[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum + byTempRxAnt * m_PathNum * s_SubPathNum + byTempPath * s_SubPathNum + byTempSubPath] *
								m_Phase[byTempTxAnt * m_RxAntNum * m_PathNum * s_SubPathNum * 2 + byTempRxAnt * m_PathNum * s_SubPathNum * 2 + byTempPath * s_SubPathNum * 2 + byTempSubPath * 2+ 1]);
				    }
				}
			}
		}


		memset(t_pfHFFT, 0, m_TxAntNum * m_RxAntNum * 1024 * 2 * sizeof(double));

		for (int byTempTxAnt = 0; byTempTxAnt != m_TxAntNum; ++byTempTxAnt)
		{
			for (int byTempRxAnt = 0; byTempRxAnt != m_RxAntNum; ++byTempRxAnt)
			{
				for (int byTempPath = 0; byTempPath != m_PathNum; ++byTempPath)
				{
					t_pfHFFT[byTempTxAnt * m_RxAntNum * m_FFTNum * 2 + byTempRxAnt * m_FFTNum * 2 + m_FFTIndex[byTempPath] * 2] +=
						t_pfH[byTempTxAnt * m_RxAntNum * m_PathNum * 2 + byTempRxAnt * m_PathNum * 2 + byTempPath * 2];
					t_pfHFFT[byTempTxAnt * m_RxAntNum * m_FFTNum * 2 + byTempRxAnt * m_FFTNum * 2 + m_FFTIndex[byTempPath] * 2 + 1] +=
						t_pfH[byTempTxAnt * m_RxAntNum * m_PathNum * 2 + byTempRxAnt * m_PathNum * 2 + byTempPath * 2 + 1];
				}
				
			}
		}


		//利用fftw提供的函数进行fft变换
		fftw_complex *in1, *out1, *in2, *out2;
		fftw_plan ptemp1, ptemp2;
		in1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_FFTNum);
		out1 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_FFTNum);
		in2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_FFTNum);
		out2 = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_FFTNum);
		for (int tempnum = 0; tempnum<1024; tempnum++)
		{
			in1[tempnum][0] = t_pfHFFT[tempnum * 2];
			in1[tempnum][1] = t_pfHFFT[tempnum * 2 + 1];
			in2[tempnum][0] = t_pfHFFT[tempnum * 2 + 2048];
			in2[tempnum][1] = t_pfHFFT[tempnum * 2 + 2049];
		}
		ptemp1 = fftw_plan_dft_1d(1024, in1, out1, FFTW_FORWARD, FFTW_ESTIMATE);
		ptemp2 = fftw_plan_dft_1d(1024, in2, out2, FFTW_FORWARD, FFTW_ESTIMATE);
		fftw_execute(ptemp1);
		fftw_execute(ptemp2);


		int HAfterFFT_Iter = 0;
		for (int row = 0; row < 1024; row++) {
			t_HAfterFFT[HAfterFFT_Iter++] = (double)out1[row][0];
			t_HAfterFFT[HAfterFFT_Iter++] = (double)out1[row][1];
		}
		for (int row = 0; row < 1024; row++) {
			t_HAfterFFT[HAfterFFT_Iter++] = (double)out2[row][0];
			t_HAfterFFT[HAfterFFT_Iter++] = (double)out2[row][1];
		}
	
		fftw_destroy_plan(ptemp1);
		fftw_destroy_plan(ptemp2);
		fftw_free(in1);
		fftw_free(in2);
		fftw_free(out1);
		fftw_free(out2);
	return;
}

void imta::refresh(){
	memory_clean::safe_delete(m_Gain, true);
	memory_clean::safe_delete(m_SinAoD, true);
	memory_clean::safe_delete(m_CosAoD, true);
	memory_clean::safe_delete(m_Phase, true);
	memory_clean::safe_delete(m_SinAoA, true);
	memory_clean::safe_delete(m_CosAoA, true);
	memory_clean::safe_delete(m_PhaseLoS, true);
	memory_clean::safe_delete(m_FFTIndex, true);
}