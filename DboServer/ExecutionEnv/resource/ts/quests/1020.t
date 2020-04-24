CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1020;
	title = 102002;

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
				tidx0 = 8031102;
				ectype = -1;
				etype = 0;
				iidx0 = 743;
				it0 = 102020;
				taid = 1;
				widx0 = 5;
				dt0 = 102021;
				esctype = 0;
				otype0 = 1;
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
			stdiag = 102007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 36;
			}
			CDboTSClickNPC
			{
				npcidx = "4072101;";
			}
			CDboTSCheckClrQst
			{
				and = "1019;";
				flink = 1;
				flinknextqid = "1045;";
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
				conv = 102009;
				ctype = 1;
				idx = 4072101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 102008;
				gtype = 1;
				area = 102001;
				goal = 102004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 102005;
				stype = 1;
				taid = 1;
				title = 102002;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 102008;
			gtype = 1;
			oklnk = 2;
			area = 102001;
			goal = 102004;
			sort = 102005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 102002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 102014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 102001;
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
			stdiag = 102007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

