CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 1;
	tid = 871;
	title = 87102;

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
				conv = 87109;
				ctype = 1;
				idx = 1653107;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 1;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 87120;
				taid = 3;
				widx0 = 3;
				dt0 = 87121;
				esctype = 0;
				otype0 = 7;
			}
			CDboTSActItem
			{
				iidx0 = 99063;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 87108;
				gtype = 1;
				area = 87101;
				goal = 87104;
				grade = 132203;
				rwd = 100;
				scitem = 99063;
				sort = 87105;
				stype = 2;
				taid = 1;
				title = 87102;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 187;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 101;
			cid = 100;
			rwdzeny = 0;
			desc = 87114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 87101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 87107;
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
				npcidx = "1653107;";
			}
			CDboTSCheckClrQst
			{
				and = "839;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 87108;
			gtype = 1;
			oklnk = 2;
			area = 87101;
			goal = 87104;
			sort = 87105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 87102;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 87107;
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

			CDboTSActItem
			{
				iidx0 = 99063;
				stype0 = 1;
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
	}
}

