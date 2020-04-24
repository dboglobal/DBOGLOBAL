CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1019;
	title = 101902;

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
				eitype = 6;
				tidx0 = 4371304;
				ectype = -1;
				etype = 0;
				iidx0 = 743;
				it0 = 101920;
				taid = 1;
				widx0 = 5;
				dt0 = 101921;
				esctype = 0;
				otype0 = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
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
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 743;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 101907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 36;
			}
			CDboTSCheckClrQst
			{
				and = "992;";
				flink = 1;
				flinknextqid = "1020;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4072101;";
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
				conv = 101909;
				ctype = 1;
				idx = 4072101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 101908;
				gtype = 1;
				area = 101901;
				goal = 101904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 101905;
				stype = 1;
				taid = 1;
				title = 101902;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 101908;
			gtype = 1;
			oklnk = 2;
			area = 101901;
			goal = 101904;
			sort = 101905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 101902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 101914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 101901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
			CDboTSClickNPC
			{
				npcidx = "4072101;";
			}
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
				iidx0 = 743;
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
			stdiag = 101907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

