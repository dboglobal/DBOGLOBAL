CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2293;
	title = 229302;

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
				conv = 229309;
				ctype = 1;
				idx = 7133104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 7637103;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 229320;
				taid = 3;
				widx0 = 1;
				dt0 = 229321;
				esctype = 0;
				otype0 = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 229308;
				gtype = 2;
				area = 229301;
				goal = 229304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 229305;
				stype = 2;
				taid = 1;
				title = 229302;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1406;
				taid = 1;
				type = 0;
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
				iidx0 = 1406;
				taid = 2;
				type = 1;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 229308;
			gtype = 2;
			oklnk = 2;
			area = 229301;
			goal = 229304;
			sort = 229305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 229302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 229314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 229301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1406;
			}
			CDboTSClickNPC
			{
				npcidx = "7133104;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 229307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "7133104;";
			}
			CDboTSCheckClrQst
			{
				and = "2292;";
				flink = 1;
				flinknextqid = "2294;";
				not = 0;
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
				iidx0 = 1406;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 229307;
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
	}
}

