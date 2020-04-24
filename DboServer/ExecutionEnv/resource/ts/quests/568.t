CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 568;
	title = 56802;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 56809;
				ctype = 1;
				idx = 3142109;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 56808;
				m1fx = "6754.000000";
				m1fy = "0.000000";
				sort = 56805;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "-4270.000000";
				grade = 132203;
				m0fx = "-191.190002";
				m0ttip = 56815;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 56802;
				gtype = 3;
				area = 56801;
				goal = 56804;
				m0fz = "510.429993";
				m0widx = 7;
				m1ttip = 56816;
				scitem = -1;
				stype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 5811110;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 56808;
			gtype = 3;
			oklnk = 2;
			area = 56801;
			goal = 56804;
			sort = 56805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 56802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 56814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 56801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142109;";
			}
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
			stdiag = 56807;
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
				minlvl = 20;
			}
			CDboTSClickNPC
			{
				npcidx = "3142109;";
			}
			CDboTSCheckClrQst
			{
				and = "562;";
				flink = 1;
				flinknextqid = "569;";
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

