CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1451;
	title = 145102;

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
				eitype = 1;
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
			stdiag = 145107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 46;
			}
			CDboTSClickNPC
			{
				npcidx = "1651102;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1476;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 145109;
				ctype = 1;
				idx = 1651102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 1;
				idx0 = 826;
				cnt0 = 4;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 827;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 145108;
				m1fx = "428.000000";
				m1fy = "0.000000";
				sort = 145105;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "122.000000";
				grade = 132203;
				m0fx = "255.000000";
				m0ttip = 145115;
				m1pat = 2;
				m1widx = 2;
				rwd = 100;
				taid = 1;
				title = 145102;
				gtype = 3;
				area = 145101;
				goal = 145104;
				m0fz = "536.000000";
				m0widx = 2;
				m1ttip = 145116;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 145108;
			gtype = 3;
			oklnk = 2;
			area = 145101;
			goal = 145104;
			sort = 145105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 145102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 145114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 145101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "1651102;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 827;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 826;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 145107;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
	}
}

