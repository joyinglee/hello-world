{
	char *name = "LYSO scintillator";
	char *listing = "LYSO";
	char *upcom = "乱码";
	char *downcom = "emmmm";
	
	gROOT->ProcessLine(".x InitSLitrani.C(5,name,listing,upcom,downcom,kFALSE,kTRUE,kFALSE)");
	gCleanOut->fMaxInfo =1000;
	
	
	
	//////////////////////////////////////
	//参数列表
	//////////////////////////////////////
	Int_t mediumindex = 1;
	//Double_t a = 5.0;
	Double_t hole_num = 10;	//孔的列/行数
	Double_t hole_dis = 0.000152;	//孔的距离
	Double_t hole_r = 0.0000152;	//孔的半径
	Double_t layer_dz = 0.0000152;	//涂层的厚度
	Double_t crys_d = 5.0;	//晶体的大小
	Double_t apd_dz = 1.0;	//apd的半高
	
	
	TGeoManager *geom = new TGeoManager("setup","LYSO simulation");
	
	
	//////////////////////////////////////
	//元素表
	//////////////////////////////////////
	/*
	TGeoElementTable *table = gGeoManager->GetElementTable();
	//TGeoElement * = table->FindElement();
	TGeoElement *Lu = table->FindElement("LUTETIUM");
	TGeoElement *Y = table->FindElement("YTTRIUM");
	TGeoElement *Si = table->FindElement("SILICON");
	TGeoElement *O = table->FindElement("OXYGEN");
	TGeoElement *Ce = table->FindElement("CERIUM");
	
	TGeoMixture *lyso_mix = new TGeoMixture("lyso",1.83,7.15);	//名字,折射率?,密度
	lyso_mix->AddElement(Lu,2);
	lyso_mix->AddElement(Y,2);
	lyso_mix->AddElement(Si,1);
	lyso_mix->AddElement(O,5);
	lyso_mix->AddElement(Ce,1);
	lyso_mix->SetRadLen(-1.10,19.5);		//辐射波长,相互作用长度(interaction length)(PbWO4)
	*/
	TGeoElementTable *table = gGeoManager->GetElementTable();
	TGeoElement *Pb = table->FindElement("LEAD");
	TGeoElement *W  = table->FindElement("TUNGSTEN");
	TGeoElement *O  = table->FindElement("OXYGEN");
	TGeoMixture *pbwo4_mix = new TGeoMixture("PbWO4",3,8.28);
	pbwo4_mix->AddElement(Pb,1);
	pbwo4_mix->AddElement(W,1);
	pbwo4_mix->AddElement(O,4);
	pbwo4_mix->SetRadLen(-1.10,19.5);
	
	
	//////////////////////////////////////
	//各类材料
	//////////////////////////////////////
	//铝
	const Double_t Air_RefrIndex     = 1.0003;   //Refractive index of air
	const Double_t aluminium_diffus  = 0.01;
	const Double_t AluminiumSupplAbs = 0.1;
	TGeoMaterial *aluminium_mat = new TGeoMaterial("Aluminium",0.0,0.0,0.0);
	TLitMedium *aluminium = new TLitMedium("Aluminium",mediumindex++,aluminium_mat,1.0);
	aluminium->SetAsWrapping(aluminium_diffus,"RIndexRev_Aluminium","IIndexRev_Aluminium",1.0,AluminiumSupplAbs);
	aluminium->IsIsotropic(Air_RefrIndex);
	//真空
	TGeoMaterial *vacuum_mat = new TGeoMaterial("vacuum",0.0,0.0,0.0);	//irrelevant(不相干的)参数
	TLitMedium *vacuum = new TLitMedium("vacuum",mediumindex++,vacuum_mat,kFALSE,1.0,100000.0);
	vacuum->IsIsotropic(1.0);
	//晶体材料
	//TLitMedium *lyso = new TLitMedium("lyso",mediumindex++,lyso_mix,kFALSE,1.0,"AbsorptionLength_LYSO"); 
	TLitMedium *lyso = new TLitMedium("lyso",mediumindex++,pbwo4_mix,kFALSE,1.0,"AbsorptionLength_PbWO4"); 
	lyso->IsIsotropic(1,1.5861,1.1062,27.063);
	lyso->FindSpectrum("Spectrum_PbWO4La");
	//pbwo4->SetCascades(2.0);
	//apd
	TGeoElement *Si = table->FindElement("SILICON");
	TGeoMaterial *silicium_mat = new TGeoMaterial("Silicium",Si,2.33);
	TLitMedium *silicium = new TLitMedium("Silicium",mediumindex++,silicium_mat,kTRUE,1.0,"AbsorptionLength_Silicium");
	silicium->IsIsotropic("RefrIndex_Silicium");
	//完全吸收层
	TGeoMaterial *totabs_mat = new TGeoMaterial("TotAbsorbing",0.0,0.0,0.0);
	TLitMedium *totabsorbing = new TLitMedium("TotAbsorbing",mediumindex++,totabs_mat,-1.0);
	totabsorbing->SetAsWrapping(0.0,0.0,1.0,1.0,1.0);
	totabsorbing->IsIsotropic(1.0);
	
	
	////////////////////////////////////////////
	//位置信息
	////////////////////////////////////////////
	TGeoTranslation *t1 = new TGeoTranslation("t1",0.0,0.0,crys_d+layer_dz);
	t1->RegisterYourself();
	TGeoTranslation *t0 = new TGeoTranslation("t0",0.0,0.0,0.0);
	
	TGeoTranslation *t_apd = new TGeoTranslation("t_apd",hole_num*hole_dis/2.0,hole_num*hole_dis/2.0,crys_d+(layer_dz*2.0)+apd_dz);	//孔的起始位置在0，0处，因此需要计算孔阵列的总中心
	
	Double_t phi,theta,psi;
	TGeoRotation r1;
	TEuler euler(90.0,0.0,180.0,90.0);
	euler.GetEulerAngles(phi,theta,psi);
	r1.SetAngles(phi,theta,psi);
	TGeoTranslation t_source(0.0,-crys_d-2,0.0);
	TGeoCombiTrans *c1 = new TGeoCombiTrans(t_source,r1);
	TGeoHMatrix *ph_source = new TGeoHMatrix(*c1);
	//TGeoTranslation *t_source = new TGeoTranslation("CRADLE",0.0,crys_d+1,0.0);
	
	
	//////////////////////////////////////////////
	//几何图形的搭建
	//////////////////////////////////////////////
	TGeoVolume *top = geom->MakeBox("TOP",vacuum,10,10,10);
	geom->SetTopVolume(top);
	
	TGeoVolume *crys = geom->MakeBox("LYSO",lyso,5,5,5);
	top->AddNode(crys,1,t0);
	
	TGeoVolume *layer_base = geom->MakeBox("LAYER_BASE",lyso,5,5,0.0000152);
	top->AddNode(layer_base,1,t1);
	
	//rmin,rmax,dz,phi_op,phi_ed
	TGeoVolume *layer_hole = geom->MakeTubs("TUB1",vacuum,0,hole_r,0.0000152,0,360);
	//layer_base->AddNode(layer_hole,1,t0);	//独孔测试
	
	Double_t a,b,a1,b1;
	for (a=0;a<=hole_num;a++)	//多孔阵列的排布
	{
		for(b=0;b<=hole_num;b++)
		{
			a1 = a*hole_dis;
			b1 = b*hole_dis;
			TGeoTranslation *tt = new TGeoTranslation("",b1,a1,0.0);
			layer_base->AddNode(layer_hole,1,tt);
		}
	}
	
	TGeoVolume *apd = geom->MakeBox("APD",silicium,a1/2.0,b1/2.0,apd_dz);
	//TGeoVolume *apd = geom->MakeBox("APD",silicium,1.0,1.0,1);//test
	top->AddNode(apd,1,t_apd);
	
	TLitVolume *lit_apd = new TLitVolume(apd);
	lit_apd->SetAPD("GainProfile_CMSAPD",3);//unknown
	
	TGeoVolume *source = geom->MakeEltu("source",vacuum,1.0,1.0,1.0);
	top->AddNode(source,1,ph_source);
	
	
	/////////////////////////////////////////
	//其他参数
	/////////////////////////////////////////
	geom->CloseGeometry();
	geom->CheckOverlaps(0.0000001);//warnning
	
	
	
	gGeoManager->SetTopVisible(1);
	gGeoManager->SetVisLevel(4);
	TLit::Get()->BookCanvas(5);
	gTwoPad->SetStateOfPads(Pad1AndPad2);
	gTwoPad->SetAllGrey();
	const char *comTL = "This is the comTL string";
	gTwoPad->CommentTL(comTL);
	
	
	
	top->Draw("");
	
	// TVector3 dir(0.0,0.0,1.0);
	// TVector3 dirfce(0.0,0.0,1.0);
	// TLitParticle *particle = new TLitParticle(13,gaussian,100.0,90.0,110.0,5.0);
	// TLitBeam *beam = new TLitBeam("muons","beam of muons","/TOP_1/APD_1",particle,kTRUE,kFALSE);
	// beam->SetEmission(flat,5.0,dir,"",kFALSE,0.0,kTRUE,dirfce);
	//GeV
	
	TLitParticle *particle = new TLitParticle(13,gaussian,100.0,90.0,110.0,5.0);
	//TLitParticle *particle = new TLitParticle(34,gaussian,0.005430);
	TVector3 dir(0.0,0.0,1.0);
	TLitBeam *beam = new TLitBeam("muons","beam of muons","/TOP_1/source_1",particle);
	beam->SetEmission(flat,5.0,dir,"",kFALSE,0.0,kFALSE,dir);
	beam->Gen(1,1000);
	
	// TLitGamma *gamma =new TLitGamma("gamma","gamma title","/TOP_1/APD_1",5.43);
	// TVector3 dir(0.0,-1.0,0.0);
	// gamma->SetEmission(on4pi,90,dir);
	// gamma->Gen(1,1000);
	
	gLitGp->SetTitle("gLitGp title");
	gLitGp->Summary();
	gTwoPad->SetStateOfPads(Pad1AndPad2);
	gTwoPad->ChangePad();
	gLitGs->DoStat();
	
	TLit::Get()->CloseFiles();
}
