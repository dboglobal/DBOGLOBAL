CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1553;
	title = 155302;

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
			stdiag = 155307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "4152109;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1555;";
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
				conv = 155309;
				ctype = 1;
				idx = 4152109;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 1;
				idx0 = 938;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 939;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 155308;
				m1fx = "-2098.000000";
				m1fy = "0.000000";
				sort = 155305;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1371.000000";
				grade = 132203;
				m0fx = "-2124.000000";
				m0ttip = 155315;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 155302;
				gtype = 2;
				area = 155301;
				goal = 155304;
				m0fz = "1565.000000";
				m0widx = 1;
				m1ttip = 155316;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 155314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 155301;
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
				npcidx = "4152109;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 155308;
			gtype = 2;
			oklnk = 2;
			area = 155301;
			goal = 155304;
			sort = 155305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 155302;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 155307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 938;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 720;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 0;
		}
	}
}

