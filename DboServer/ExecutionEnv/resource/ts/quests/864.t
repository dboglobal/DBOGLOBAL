CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 864;
	title = 86402;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 86408;
			gtype = 3;
			oklnk = 2;
			area = 86401;
			goal = 86404;
			sort = 86405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 86402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 86414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 86401;
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
				npcidx = "1653105;";
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
			stdiag = 86407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSClickNPC
			{
				npcidx = "1653101;";
			}
			CDboTSCheckClrQst
			{
				and = "863;";
				flink = 1;
				flinknextqid = "866;";
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
				conv = 86409;
				ctype = 1;
				idx = 1653101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 16;
				eitype = 1;
				idx0 = 600;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 601;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 86408;
				m3widx = 3;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 3;
				m3fx = "623.000000";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				m4fx = "572.000000";
				m4ttip = 86416;
				area = 86401;
				goal = 86404;
				m0fz = "-351.000000";
				m0widx = 3;
				m1ttip = 86416;
				m2fy = "0.000000";
				m2pat = 0;
				m3fz = "-194.000000";
				m3ttip = 86415;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-351.000000";
				m2fx = "7448.000000";
				m2ttip = 86417;
				m4widx = 3;
				m0fx = "403.000000";
				m0ttip = 86415;
				m3pat = 2;
				m4pat = 2;
				title = 86402;
				m1fx = "403.000000";
				m2fz = "1558.000000";
				sort = 86405;
				m4fz = "-386.000000";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 86407;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
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
				iidx1 = 601;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 600;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
	}
}

