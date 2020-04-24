CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 6114;
	title = 611402;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 611408;
			gtype = 1;
			oklnk = 2;
			area = 611401;
			goal = 611404;
			sort = 611405;
			prelnk = "0;";
			ds = 0;
			grade = 157103;
			rwd = 100;
			title = 611402;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 611409;
				ctype = 1;
				idx = 2413112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 611408;
				gtype = 1;
				area = 611401;
				goal = 611404;
				grade = 157103;
				rwd = 100;
				scitem = -1;
				sort = 611405;
				stype = 4;
				taid = 1;
				title = 611402;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92049;
				eitype = 4;
				idx0 = 1562;
				rate0 = "1.000000";
				scd0 = 5;
				cnt0 = 30;
				ectype = 0;
				etype = 0;
				nidx0 = -1;
				taid = 3;
				esctype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 611407;
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
				npcidx = "2413112;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 611414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 611401;
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
				npcidx = "2413112;";
			}
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
				iidx0 = 1562;
				taid = 1;
				type = 1;
			}
		}
	}
}

