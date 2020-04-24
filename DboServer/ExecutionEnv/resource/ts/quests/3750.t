CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 3750;
	title = 375002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 375007;
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
				npcidx = "7072301;";
			}
			CDboTSCheckClrQst
			{
				and = "3749;";
				flink = 0;
				flinknextqid = "3751;";
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
				conv = 375009;
				ctype = 1;
				idx = 7072301;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 40;
				eitype = 0;
				idx0 = 18481100;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 19431100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 375008;
				m3widx = 15;
				m1fy = "0.000000";
				m2widx = 15;
				grade = 132203;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-4454.520020";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-4407.520020";
				m4ttip = 375016;
				area = 375001;
				goal = 375004;
				m0fz = "-4924.930176";
				m0widx = 15;
				m1ttip = 375015;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-4590.609863";
				m3ttip = 375016;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4806.899902";
				m2fx = "-4410.850098";
				m2ttip = 375015;
				m4widx = 15;
				m0fx = "-4456.660156";
				m0ttip = 375015;
				m3pat = 2;
				m4pat = 2;
				title = 375002;
				m1fx = "-4391.229980";
				m2fz = "-4645.069824";
				sort = 375005;
				m4fz = "-4825.220215";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 375008;
			gtype = 2;
			oklnk = 2;
			area = 375001;
			goal = 375004;
			sort = 375005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 375002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 375014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 375001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1163101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
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

