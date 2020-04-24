CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2622;
	title = 262202;

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
				conv = 262209;
				ctype = 1;
				idx = 5313102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 262208;
				gtype = 2;
				area = 262201;
				goal = 262204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 262205;
				stype = 1;
				taid = 1;
				title = 262202;
			}
			CDboTSActSToCEvt
			{
				dt2 = 262225;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 262221;
				esctype = 0;
				iidx1 = -1;
				it1 = 262222;
				otype0 = 1;
				tidx2 = 4142109;
				dt1 = 262223;
				etype = 0;
				iidx2 = -1;
				it0 = 262220;
				taid = 3;
				tidx1 = 4501106;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 5592107;
				ectype = -1;
				it2 = 262224;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 262214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 262201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5313102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 262208;
			gtype = 2;
			oklnk = 2;
			area = 262201;
			goal = 262204;
			sort = 262205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 262202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 262207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5313102;";
			}
			CDboTSCheckClrQst
			{
				and = "2621;";
				flink = 1;
				flinknextqid = "2623;";
				not = 0;
			}
		}
	}
}

