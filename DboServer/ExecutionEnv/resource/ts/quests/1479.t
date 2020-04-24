CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1479;
	title = 147902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 147909;
				ctype = 1;
				idx = 2803101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 147908;
				gtype = 2;
				area = 147901;
				goal = 147904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 147905;
				stype = 1;
				taid = 1;
				title = 147902;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 2;
				idx0 = 845;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 846;
				taid = 3;
				esctype = 0;
				idx2 = -1;
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
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 846;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 845;
				iprob1 = "1.000000";
				taid = 2;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 147908;
			gtype = 2;
			oklnk = 2;
			area = 147901;
			goal = 147904;
			sort = 147905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 147902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 147914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 147901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 846;
			}
			CDboTSClickNPC
			{
				npcidx = "1651102;";
			}
			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 845;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 147907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 46;
			}
			CDboTSCheckClrQst
			{
				and = "1478;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2803101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 846;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 845;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
	}
}

