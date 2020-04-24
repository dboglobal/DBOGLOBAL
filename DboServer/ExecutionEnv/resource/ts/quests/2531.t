CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2531;
	title = 253102;

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
				conv = 253109;
				ctype = 1;
				idx = 7711203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 253108;
				gtype = 2;
				area = 253101;
				goal = 253104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 253105;
				stype = 1;
				taid = 1;
				title = 253102;
			}
			CDboTSActSToCEvt
			{
				dt2 = 253125;
				otype2 = 1;
				widx1 = 14;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 14;
				dt0 = 253121;
				esctype = 0;
				iidx1 = -1;
				it1 = 253122;
				otype0 = 1;
				tidx2 = 4142110;
				dt1 = 253123;
				etype = 0;
				iidx2 = -1;
				it0 = 253120;
				taid = 3;
				tidx1 = 4651106;
				widx0 = 14;
				apptype = 0;
				eitype = 6;
				tidx0 = 6313111;
				ectype = -1;
				it2 = 253124;
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
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
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
				itype = 6;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 253114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 253101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7711203;";
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
			cont = 253108;
			gtype = 2;
			oklnk = 2;
			area = 253101;
			goal = 253104;
			sort = 253105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 253102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 253107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "7711203;";
			}
			CDboTSCheckClrQst
			{
				and = "2530;";
				flink = 1;
				flinknextqid = "2532;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
}

