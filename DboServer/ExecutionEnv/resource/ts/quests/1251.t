CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1251;
	title = 125102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 100;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 407;
				cnt0 = 15;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

			CDboTSActSendSvrEvt
			{
				id = 110;
				tblidx = 1353102;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 0;
			}
			CDboTSActSendSvrEvt
			{
				id = 111;
				tblidx = 5533402;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 125109;
				ctype = 1;
				idx = 1353102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 125108;
				m1fx = "2720.000000";
				m1fy = "0.000000";
				sort = 125105;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "3651.000000";
				grade = 132203;
				m0fx = "2379.000000";
				m0ttip = 125115;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 125102;
				gtype = 2;
				area = 125101;
				goal = 125104;
				m0fz = "3435.000000";
				m0widx = 1;
				m1ttip = 125115;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 125107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSClickNPC
			{
				npcidx = "1353102;";
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
			cont = 125108;
			gtype = 2;
			oklnk = 2;
			area = 125101;
			goal = 125104;
			sort = 125105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 125102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 125114;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 125101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "1353102;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;";
			type = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 255;
			prelnk = "1;2;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 407;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 255;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 407;
				taid = 1;
				type = 1;
			}
		}
	}
}

