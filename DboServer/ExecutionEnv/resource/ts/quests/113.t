CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 18;
	rq = 0;
	tid = 113;
	title = 11302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 11314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 11301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6413107;";
			}
			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 11308;
			gtype = 2;
			oklnk = 2;
			area = 11301;
			goal = 11304;
			sort = 11305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 11302;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 11307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "116;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "6413107;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 11309;
				ctype = 1;
				idx = 6413107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 11308;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 11305;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "5892.879883";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 11301;
				goal = 11304;
				m0fz = "4655.669922";
				m0widx = 1;
				m1ttip = 11316;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "4402.129883";
				m3ttip = 11316;
				scitem = -1;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "4123.459961";
				m2fx = "6099.529785";
				m2ttip = 11315;
				m0fx = "5507.049805";
				m0ttip = 11315;
				m3pat = 2;
				title = 11302;
				m1fx = "6099.529785";
				m2fz = "4123.459961";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 1;
				idx0 = 1198;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1199;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
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
				eitype = 1;
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
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 11307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 1199;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 1198;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
	}
}

