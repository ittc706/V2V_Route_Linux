#pragma once
#include<math.h>
#include"fftw3.h"

enum location_type { //位置类型 
	Los,
	Nlos,
	None
};

struct location {
	bool manhattan;
	location_type locationType;
	double distance; //单位:m
	double distance1; //单位:m
	double distance2; //单位:m
	double eNBAntH; //单位:m
	double VeUEAntH; //单位:m
	double RSUAntH;//单位：m
	double posCor[5];

};

struct antenna {
	double TxAngle; // degree
	double RxAngle; // degree
	double maxAttenu; // dB
	int byTxAntNum;
	int byRxAntNum;
	double *TxSlantAngle; // degree
	double *RxSlantAngle; // degree
	double *TxAntSpacing;
	double *RxAntSpacing;
	double antGain;
};

/*===========================================
*               imta信道模型
* ==========================================*/
class imta {
	/*------------------静态------------------*/
public:
	static const double s_PI;
	static const double s_PINeg;
	static const double s_PI2;
	static const double s_PI_HALF;
	static const double s_DEGREE_TO_PI;
	static const double s_SQRT_HALF;
	static const double s_SQRT_THREE;
	static const double s_C;
	static const double s_FC;
public:
	//路径数
	static const int s_SubPathNum = 20;
	static const int s_MidPathNum = 3;
	//相关系数矩阵
	static const double s_ConstantInHLoS[25];
	static const double s_ConstantInHNLoS[25];
	static const double s_ConstantUMiLoS[25];
	static const double s_ConstantUMiNLoS[25];
	static const double s_ConstantUMiO2I[25];
	static const double s_ConstantSMaLoS[25];
	static const double s_ConstantSMaNLoS[25];
	static const double s_ConstantUMaLoS[25];
	static const double s_ConstantUMaNLoS[25];
	static const double s_ConstantRMaLoS[25];
	static const double s_ConstantRMaNLoS[25];
	//角度偏移数组
	static const double s_AngleOffset[s_SubPathNum];
	static const double s_MidPathDelayOffset[s_MidPathNum];
	static const int s_MidPathIndex[s_SubPathNum];

	static void randomGaussian(double *t_pfArray, long t_ulNumber, double t_fMean, double t_fStandardDeviation);
	static void randomUniform(double *t_pfArray, long t_ulNumber, double t_fUpBound, double t_fDownBound, bool t_bFlagZero);
	static void sortBubble(double *t_pfArray, int t_wNumber, bool t_bFlagDirection, bool t_bFlagFabs);
	static void selectMax(double *t_pfArray, int t_byNumber, int *t_pbyFirst, int *t_pbySecond);

	/*------------------域------------------*/
	//信道所需基本常量
	double m_AntGain;
	double m_MaxAttenu; // dBm
	int m_TxAntNum;
	int m_RxAntNum;
	double * m_TxSlantAngle; // degree
	double * m_RxSlantAngle; // degree
	double * m_TxAntSpacing;
	double * m_RxAntSpacing;
	double m_TxAngle;
	double m_RxAngle;

	//信道所需配置常量
	int m_PathNum;
	double m_Velocityi;
	double m_Velocityj;
	double m_VAnglei;
	double m_VAnglej;
	double m_C;
	double m_PathShadowSTD;
	double m_AoDRatio;
	double m_AoARatio;
	double m_XPR;
	double m_DS;
	double m_DSRatio;
	double m_AoD;
	double m_AoA;
	double m_K;
	double m_KDB;
	double m_D;


	bool m_LoS;
	bool m_Built;
	bool m_Enable;
	double m_PLSF;

	int m_PathFirst;
	int m_PathSecond;
	//信道所需存储常量
	double *m_Gain;
	double *m_SinAoD;
	double *m_CosAoD;
	double *m_SinAoA;
	double *m_CosAoA;
	double *m_Phase;

	double m_GainLoS;
	double m_SinAoDLoS;
	double m_fSinAoALoS;
	double m_CosAoALoS;
	double *m_PhaseLoS;

	//FFT所用变量
	int m_FFTNum;
	int m_FFTOrder;
	double m_FFTTime;
	int m_HNum;
	int *m_FFTIndex;
public:
	imta();
	~imta();
	bool build(double* t_Pl, double t_fFrequency/*Hz*/, location &t_eLocation, antenna &t_eAntenna,  double t_fVelocityi/*km/h*/, double t_fVelocityj/*km/h*/, double t_fVAnglei/*degree*/, double t_fVAnglej/*degree*/);
	bool enable(bool *t_pbEnable);
	void calculate(double* t_HAfterFFT, double t_fT/*s*/, double *t_pfTemp, double *t_pfSin, double *t_pfCos, double *t_pfH, double *t_pfHFFT);
	void refresh();
};