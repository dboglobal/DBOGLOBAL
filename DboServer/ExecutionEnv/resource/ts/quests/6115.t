CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 6115;
	title = 611502;

	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 611507;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1580;
				taid = 1;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 4;
				idx0 = -1;
				rate0 = "1.000000";
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				nidx0 = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
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
			desc = 611514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 611501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "2413113;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 611509;
				ctype = 1;
				idx = 2413113;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92070;
				eitype = 4;
				idx0 = 1580;
				rate0 = "1.000000";
				scd0 = 5;
				cnt0 = 30;
				ectype = 0;
				etype = 0;
				nidx0 = -1;
				taid = 3;
				esctype = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 611508;
				gtype = 1;
				area = 611501;
				goal = 611504;
				grade = 157103;
				rwd = 100;
				scitem = -1;
				sort = 611505;
				stype = 4;
				taid = 1;
				title = 611502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 611508;
			gtype = 1;
			oklnk = 2;
			area = 611501;
			goal = 611504;
			sort = 611505;
			prelnk = "0;";
			ds = 0;
			grade = 157103;
			rwd = 100;
			title = 611502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 611507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "2413113;";
			}
		}
	}
}

