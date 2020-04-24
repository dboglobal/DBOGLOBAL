CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 3739;
	title = 373902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 373907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "5591212;";
			}
			CDboTSCheckClrQst
			{
				and = "3738;";
				flink = 0;
				flinknextqid = "3740;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 373909;
				ctype = 1;
				idx = 5591212;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 35;
				eitype = 0;
				idx0 = 14051100;
				cnt0 = 35;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 14051200;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 373908;
				m3widx = 15;
				m1fy = "0.000000";
				sort = 373905;
				m2widx = 15;
				grade = 132203;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-4830.955078";
				rwd = 100;
				taid = 1;
				gtype = 1;
				m3fy = "0.000000";
				area = 373901;
				goal = 373904;
				m0fz = "-3793.416992";
				m0widx = 15;
				m1ttip = 373915;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-3793.416992";
				m3ttip = 373916;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3804.550049";
				m2fx = "-4944.390137";
				m2ttip = 373916;
				m0fx = "-4830.955078";
				m0ttip = 373915;
				m3pat = 2;
				title = 373902;
				m1fx = "-4944.390137";
				m2fz = "-3804.550049";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 373908;
			gtype = 1;
			oklnk = 2;
			area = 373901;
			goal = 373904;
			sort = 373905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 373902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 373914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 373901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5592111;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

