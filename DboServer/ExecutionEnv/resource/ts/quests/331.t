CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 331;
	title = 33102;

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
				cnt1 = 15;
				eitype = 0;
				idx0 = 5612101;
				cnt0 = 15;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = 5613101;
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
			stdiag = 33107;
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
				minlvl = 18;
			}
			CDboTSClickNPC
			{
				npcidx = "4511602;";
			}
			CDboTSCheckClrQst
			{
				and = "330;";
				flink = 1;
				flinknextqid = "334;";
				not = 0;
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
				conv = 33109;
				ctype = 1;
				idx = 4511602;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 5612101;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5613101;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 33108;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 33105;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "4243.069824";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 33101;
				goal = 33104;
				m0fz = "-1643.760010";
				m0widx = 1;
				m1ttip = 33115;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "1383.150024";
				m3ttip = 33116;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1383.150024";
				m2fx = "4198.740234";
				m2ttip = 33116;
				m0fx = "4198.740234";
				m0ttip = 33115;
				m3pat = 2;
				title = 33102;
				m1fx = "4243.069824";
				m2fz = "-1643.760010";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 33114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 33101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511602;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 33108;
			gtype = 2;
			oklnk = 2;
			area = 33101;
			goal = 33104;
			sort = 33105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 33102;
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

