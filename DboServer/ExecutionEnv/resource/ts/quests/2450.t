CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2450;
	title = 245002;

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
			stdiag = 245007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "2611205;";
			}
			CDboTSCheckClrQst
			{
				and = "2448;";
				flink = 1;
				flinknextqid = "2451;";
				not = 0;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 245014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 245001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611205;";
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
				conv = 245009;
				ctype = 1;
				idx = 2611205;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 3651104;
				cnt0 = 5;
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
				cont = 245008;
				m1fx = "218.000000";
				m2fz = "1038.000000";
				m1fy = "0.000000";
				sort = 245005;
				m2widx = 14;
				grade = 132203;
				m1pat = 2;
				m1widx = 14;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 245001;
				goal = 245004;
				m0fz = "899.000000";
				m0widx = 14;
				m1ttip = 245015;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1092.000000";
				m2fx = "55.000000";
				m2ttip = 245015;
				m0fx = "153.000000";
				m0ttip = 245015;
				title = 245002;
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
			cont = 245008;
			gtype = 2;
			oklnk = 2;
			area = 245001;
			goal = 245004;
			sort = 245005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 245002;
		}
	}
}

