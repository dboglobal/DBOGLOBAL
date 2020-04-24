CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2644;
	title = 264402;

	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 264407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98023;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;5;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;";
			nolnk = 254;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 98023;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 98023;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
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
			stdiag = 264407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				and = "2643;";
				flink = 1;
				flinknextqid = "2645;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4511518;";
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
				conv = 264409;
				ctype = 1;
				idx = 4511518;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 98023;
				stype0 = 1;
				taid = 3;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 264408;
				area = 264401;
				goal = 264404;
				m0fz = "-2629.959961";
				m0widx = 1;
				scitem = -1;
				sort = 264405;
				stype = 2;
				m0fy = "-62.039001";
				m0pat = 0;
				grade = 132203;
				m0fx = "454.342010";
				m0ttip = 264415;
				rwd = 100;
				taid = 1;
				title = 264402;
				gtype = 3;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 264414;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 264401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511518;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 264408;
			gtype = 3;
			oklnk = 2;
			area = 264401;
			goal = 264404;
			sort = 264405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 264402;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 581;
			}
		}
	}
}

