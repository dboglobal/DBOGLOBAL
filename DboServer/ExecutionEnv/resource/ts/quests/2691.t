CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2691;
	title = 269102;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 20;
			idx = 26911;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
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
			stdiag = 269107;
			nolnk = 253;
			rm = 0;
			yeslnk = 20;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5553103;";
			}
			CDboTSCheckClrQst
			{
				and = "2663;";
				flink = 0;
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
				conv = 269109;
				ctype = 1;
				idx = 5553103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 12;
				eitype = 0;
				idx0 = 5882100;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5891100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 269108;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 269105;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "1088.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 269101;
				goal = 269104;
				m0fz = "-2572.000000";
				m0widx = 1;
				m1ttip = 269116;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-2698.000000";
				m3ttip = 269115;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2572.000000";
				m2fx = "1088.000000";
				m2ttip = 269116;
				m0fx = "934.000000";
				m0ttip = 269115;
				m3pat = 2;
				title = 269102;
				m1fx = "934.000000";
				m2fz = "-2698.000000";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 269114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 269101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5553103;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 269108;
			gtype = 2;
			oklnk = 2;
			area = 269101;
			goal = 269104;
			sort = 269105;
			prelnk = "20;20;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 269102;
		}
	}
}

