CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 992;
	title = 99202;

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
				conv = 99209;
				ctype = 1;
				idx = 4072101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 99208;
				gtype = 1;
				area = 99201;
				goal = 99204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 99205;
				stype = 1;
				taid = 1;
				title = 99202;
			}
		}
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
				tidx0 = 4651202;
				ectype = -1;
				etype = 0;
				iidx0 = 743;
				it0 = 99220;
				taid = 1;
				widx0 = 5;
				dt0 = 99221;
				esctype = 0;
				otype0 = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 99214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 99201;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 99207;
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
				flink = 1;
				flinknextqid = "1019;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4072101;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 99208;
			gtype = 1;
			oklnk = 2;
			area = 99201;
			goal = 99204;
			sort = 99205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 99202;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

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
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 99207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 743;
			}
		}
	}
}

