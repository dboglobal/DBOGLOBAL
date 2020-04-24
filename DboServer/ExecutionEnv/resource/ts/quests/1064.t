CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1064;
	title = 106402;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 106408;
			gtype = 2;
			oklnk = 2;
			area = 106401;
			goal = 106404;
			sort = 106405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 106402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 106414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 106401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071115;";
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
			stdiag = 106407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1065;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4071115;";
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
				conv = 106409;
				ctype = 1;
				idx = 4071115;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 0;
				idx0 = 8911201;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 8913201;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 106408;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "2654.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "2605.000000";
				m4ttip = 106416;
				area = 106401;
				goal = 106404;
				m0fz = "1761.000000";
				m0widx = 1;
				m1ttip = 106416;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "2140.000000";
				m3ttip = 106416;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1761.000000";
				m2fx = "2654.000000";
				m2ttip = 106415;
				m4widx = 1;
				m0fx = "2575.000000";
				m0ttip = 106415;
				m3pat = 2;
				m4pat = 2;
				title = 106402;
				m1fx = "2575.000000";
				m2fz = "2140.000000";
				sort = 106405;
				m4fz = "2400.000000";
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

