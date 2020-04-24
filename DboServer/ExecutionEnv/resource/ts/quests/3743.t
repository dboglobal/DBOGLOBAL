CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3743;
	title = 374302;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 374307;
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
				npcidx = "2532149;";
			}
			CDboTSCheckClrQst
			{
				and = "3742;";
				flink = 0;
				flinknextqid = "3744;";
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
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 374309;
				ctype = 1;
				idx = 2532149;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 1;
				idx0 = 1624;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 1625;
				taid = 3;
				esctype = 0;
				idx2 = 1626;
			}
			CDboTSActRegQInfo
			{
				cont = 374308;
				m1fx = "-5277.000000";
				m2fz = "-4052.000000";
				m1fy = "0.000000";
				sort = 374305;
				m2widx = 15;
				grade = 132203;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 374301;
				goal = 374304;
				m0fz = "-3912.000000";
				m0widx = 15;
				m1ttip = 374315;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4012.000000";
				m2fx = "-5217.000000";
				m2ttip = 374315;
				m0fx = "-5182.000000";
				m0ttip = 374315;
				title = 374302;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 374308;
			gtype = 2;
			oklnk = 2;
			area = 374301;
			goal = 374304;
			sort = 374305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 374302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 374314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 374301;
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
				npcidx = "2532149;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 374307;
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
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 1624;
				iidx2 = 1626;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 1625;
				iprob0 = "1.000000";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

