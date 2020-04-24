CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3764;
	title = 376402;

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
				conv = 376409;
				ctype = 1;
				idx = 3183301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 376408;
				gtype = 1;
				area = 376401;
				goal = 376404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 376405;
				stype = 2;
				taid = 1;
				title = 376402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 376407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3183301;";
			}
			CDboTSCheckClrQst
			{
				and = "3763;";
				flink = 0;
				flinknextqid = "3765;";
				not = 0;
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
			prelnk = "252;0;1;";
			type = 0;
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
			cont = 376408;
			gtype = 1;
			oklnk = 2;
			area = 376401;
			goal = 376404;
			sort = 376405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 376402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 376414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 376401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3333101;";
			}
		}
	}
}

