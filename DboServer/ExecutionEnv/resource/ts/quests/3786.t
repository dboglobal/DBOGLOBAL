CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3786;
	title = 378602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 378607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 69;
			}
			CDboTSClickNPC
			{
				npcidx = "3471301;";
			}
			CDboTSCheckClrQst
			{
				and = "3849;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 378609;
				ctype = 1;
				idx = 3471301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 378608;
				gtype = 1;
				area = 378601;
				goal = 378604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 378605;
				stype = 1;
				taid = 1;
				title = 378602;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 378614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 378601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5631101;";
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 378608;
			gtype = 1;
			oklnk = 2;
			area = 378601;
			goal = 378604;
			sort = 378605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 378602;
		}
	}
}

