CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2127;
	title = 212702;

	CNtlTSGroup
	{
		gid = 0;

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
				iidx0 = 1292;
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

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1292;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 212709;
				ctype = 1;
				idx = 1471107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 212708;
				gtype = 2;
				area = 212701;
				goal = 212704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 212705;
				stype = 2;
				taid = 1;
				title = 212702;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				tidx0 = 3331211;
				ectype = -1;
				etype = 0;
				iidx0 = -1;
				it0 = 212720;
				taid = 3;
				widx0 = 1;
				dt0 = 212721;
				esctype = 0;
				otype0 = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 212714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 212701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1292;
			}
			CDboTSClickNPC
			{
				npcidx = "1471107;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 212707;
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
				npcidx = "1471107;";
			}
			CDboTSCheckClrQst
			{
				and = "2126;";
				flink = 1;
				flinknextqid = "2128;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 212708;
			gtype = 2;
			oklnk = 2;
			area = 212701;
			goal = 212704;
			sort = 212705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 212702;
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
	}
	CNtlTSGroup
	{
		gid = 254;

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
				iidx0 = 1292;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 212707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

