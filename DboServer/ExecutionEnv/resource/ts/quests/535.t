CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 535;
	title = 53502;

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
			stdiag = 53507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "3172104;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 53514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 53501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172104;";
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
				conv = 53509;
				ctype = 1;
				idx = 3172104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 1713103;
				cnt0 = 9;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1712202;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 53508;
				m1fx = "7464.069824";
				m1fy = "0.000000";
				sort = 53505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2267.209961";
				grade = 132203;
				m0fx = "7484.890137";
				m0ttip = 53515;
				m1pat = 1;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 53502;
				gtype = 2;
				area = 53501;
				goal = 53504;
				m0fz = "-2246.649902";
				m0widx = 1;
				m1ttip = 53516;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 53508;
			gtype = 2;
			oklnk = 2;
			area = 53501;
			goal = 53504;
			sort = 53505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 53502;
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

