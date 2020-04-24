CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1900;
	title = 190002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 190009;
				ctype = 1;
				idx = 3172404;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 0;
				idx0 = 3211114;
				cnt0 = 14;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 3212113;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 190008;
				m1fx = "292.239990";
				m2fz = "94.449997";
				m1fy = "-84.110001";
				sort = 190005;
				m2widx = 4;
				grade = 132203;
				m1pat = 2;
				m1widx = 4;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 190001;
				goal = 190004;
				m0fz = "-62.169998";
				m0widx = 4;
				m1ttip = 190016;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 1;
				m0fy = "-83.029999";
				m0pat = 2;
				m1fz = "-106.470001";
				m2fx = "232.639999";
				m2ttip = 190016;
				m0fx = "268.600006";
				m0ttip = 190015;
				title = 190002;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 190014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 190001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172404;";
			}
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
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 190008;
			gtype = 2;
			oklnk = 2;
			area = 190001;
			goal = 190004;
			sort = 190005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 190002;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 190007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSCheckClrQst
			{
				and = "1899;";
				flink = 1;
				flinknextqid = "1932;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3172404;";
			}
		}
	}
}

