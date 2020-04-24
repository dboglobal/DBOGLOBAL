CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2454;
	title = 245402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 245409;
				ctype = 1;
				idx = 4511513;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 245408;
				gtype = 1;
				area = 245401;
				goal = 245404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 245405;
				stype = 2;
				taid = 1;
				title = 245402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 245407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "4511513;";
			}
			CDboTSCheckClrQst
			{
				and = "2453;";
				flink = 1;
				not = 0;
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
			prelnk = "100;";
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
			cont = 245408;
			gtype = 1;
			oklnk = 2;
			area = 245401;
			goal = 245404;
			sort = 245405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 245402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 245414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 245401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072301;";
			}
		}
	}
}

