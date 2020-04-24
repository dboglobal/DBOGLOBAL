CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1094;
	title = 109402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
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
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 109407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickNPC
			{
				npcidx = "1655301;";
			}
			CDboTSCheckClrQst
			{
				and = "1089;";
				flink = 1;
				not = 0;
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
				conv = 109409;
				ctype = 1;
				idx = 1655301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 109408;
				gtype = 2;
				area = 109401;
				goal = 109404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 109405;
				stype = 2;
				taid = 1;
				title = 109402;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				widx1 = 1;
				ectype = -1;
				iidx0 = -1;
				otype1 = 1;
				dt0 = 109421;
				esctype = 0;
				iidx1 = -1;
				it1 = 109422;
				otype0 = 1;
				dt1 = 109423;
				etype = 0;
				it0 = 109420;
				taid = 1;
				tidx1 = 8031401;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 1752101;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 109414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 109401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 109408;
			gtype = 2;
			oklnk = 2;
			area = 109401;
			goal = 109404;
			sort = 109405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 109402;
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
	}
}

