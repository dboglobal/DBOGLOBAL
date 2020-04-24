CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 608;
	title = 60802;

	CNtlTSGroup
	{
		gid = 0;

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
			stdiag = 60807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "1311203;";
			}
			CDboTSCheckClrQst
			{
				and = "607;";
				flink = 1;
				flinknextqid = "609;";
				not = 0;
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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 60809;
				ctype = 1;
				idx = 1311203;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 1412104;
				cnt0 = 8;
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
				cont = 60808;
				m1fx = "6658.080078";
				m1fy = "0.000000";
				sort = 60805;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "2214.629883";
				grade = 132203;
				m0fx = "6821.439941";
				m0ttip = 60815;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 60802;
				gtype = 2;
				area = 60801;
				goal = 60804;
				m0fz = "1975.390015";
				m0widx = 1;
				m1ttip = 60815;
				scitem = -1;
				stype = 2;
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
			cont = 60808;
			gtype = 2;
			oklnk = 2;
			area = 60801;
			goal = 60804;
			sort = 60805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 60802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 60814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 60801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1311203;";
			}
		}
	}
}

