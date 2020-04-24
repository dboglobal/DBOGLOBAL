CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 6;
	rq = 0;
	tid = 1732;
	title = 173202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1032;
				taid = 1;
				type = 0;
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
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 173207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "7135201;";
			}
			CDboTSCheckClrQst
			{
				and = "1665;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 4;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1032;
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
				conv = 173209;
				ctype = 1;
				idx = 7135201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 173201;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 173222;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 173208;
				m1fx = "-1316.199951";
				m1fy = "0.000000";
				sort = 173205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-539.159973";
				grade = 132203;
				m0fx = "-1239.800049";
				m0ttip = 173215;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 173202;
				gtype = 2;
				area = 173201;
				goal = 173204;
				m0fz = "-232.380005";
				m0widx = 1;
				m1ttip = 173215;
				scitem = -1;
				stype = 1;
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
			cont = 173208;
			gtype = 2;
			oklnk = 2;
			area = 173201;
			goal = 173204;
			sort = 173205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 173202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 173214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 173201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7135201;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1032;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 173207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

