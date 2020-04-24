CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 648;
	title = 64802;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 64809;
				ctype = 1;
				idx = 3032103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 5811108;
				cnt0 = 10;
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
				cont = 64808;
				m1fx = "6173.810059";
				m1fy = "0.000000";
				sort = 64805;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "773.169983";
				grade = 132203;
				m0fx = "5429.209961";
				m0ttip = 64815;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 64802;
				gtype = 2;
				area = 64801;
				goal = 64804;
				m0fz = "768.659973";
				m0widx = 1;
				m1ttip = 64816;
				scitem = -1;
				stype = 2;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 64808;
			gtype = 2;
			oklnk = 2;
			area = 64801;
			goal = 64804;
			sort = 64805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 64802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 64814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 64801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3032103;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 64807;
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
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "3032103;";
			}
			CDboTSCheckClrQst
			{
				and = "646;";
				flink = 1;
				flinknextqid = "649;";
				not = 0;
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
	}
}

