CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 383;
	title = 38302;

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
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 38307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 12;
			}
			CDboTSClickNPC
			{
				npcidx = "4511405;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 38309;
				ctype = 1;
				idx = 4511405;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 5613102;
				cnt0 = 9;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 38308;
				m3widx = 6;
				m1fy = "0.000000";
				sort = 38305;
				m2widx = 6;
				grade = 132203;
				m1pat = 2;
				m1widx = 6;
				m3fx = "-435.290009";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				area = 38301;
				goal = 38304;
				m0fz = "-3444.000000";
				m0widx = 1;
				m1ttip = 38316;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "1172.390015";
				m3ttip = 38316;
				scitem = -1;
				stype = 16;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "953.130005";
				m2fx = "-806.099976";
				m2ttip = 38316;
				m0fx = "4361.000000";
				m0ttip = 38315;
				m3pat = 2;
				title = 38302;
				m1fx = "-236.339996";
				m2fz = "1246.819946";
			}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 38308;
			gtype = 3;
			oklnk = 2;
			area = 38301;
			goal = 38304;
			sort = 38305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 38302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 38314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 38301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511405;";
			}
		}
	}
}

