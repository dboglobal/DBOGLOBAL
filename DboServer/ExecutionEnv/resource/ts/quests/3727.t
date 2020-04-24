CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3727;
	title = 372702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 372707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSCheckClrQst
			{
				and = "3726;";
				flink = 0;
				flinknextqid = "3728;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4952202;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372709;
				ctype = 1;
				idx = 4952202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 372708;
				gtype = 1;
				area = 372701;
				goal = 372704;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 372705;
				stype = 2;
				taid = 1;
				title = 372702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 372701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3204301;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 372708;
			gtype = 1;
			oklnk = 2;
			area = 372701;
			goal = 372704;
			sort = 372705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372702;
		}
	}
}

