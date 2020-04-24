CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2394;
	title = 239402;

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
				conv = 239409;
				ctype = 1;
				idx = 6412111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 239408;
				gtype = 2;
				area = 239401;
				goal = 239404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 239405;
				stype = 1;
				taid = 1;
				title = 239402;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1349;
				taid = 4;
				type = 0;
			}
			CDboTSActSToCEvt
			{
				dt2 = 239425;
				otype2 = 1;
				widx1 = 14;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 14;
				dt0 = 239421;
				esctype = 0;
				iidx1 = -1;
				it1 = 239422;
				otype0 = 1;
				tidx2 = 4142110;
				dt1 = 239423;
				etype = 0;
				iidx2 = -1;
				it0 = 239420;
				taid = 3;
				tidx1 = 4192108;
				widx0 = 14;
				apptype = 0;
				eitype = 6;
				tidx0 = 4651106;
				ectype = -1;
				it2 = 239424;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 239426;
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
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1349;
				taid = 2;
				type = 1;
			}
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
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 239414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 239401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1349;
			}
			CDboTSClickNPC
			{
				npcidx = "6412111;";
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
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 239407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "6412111;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2395;2396;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 239408;
			gtype = 2;
			oklnk = 2;
			area = 239401;
			goal = 239404;
			sort = 239405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 239402;
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
			cid = 254;
			prelnk = "1;";
			type = 0;
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
				iprob0 = "1.000000";
				iidx0 = 1349;
				taid = 1;
				type = 1;
			}
		}
	}
}

