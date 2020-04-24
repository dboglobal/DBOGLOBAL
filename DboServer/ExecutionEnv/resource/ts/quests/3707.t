CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3707;
	title = 370702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 370707;
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
				npcidx = "3183123;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3708;";
				not = 0;
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
				conv = 370709;
				ctype = 1;
				idx = 3183123;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 370708;
				gtype = 1;
				area = 370701;
				goal = 370704;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 370705;
				stype = 2;
				taid = 1;
				title = 370702;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 370714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 370701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3189108;";
			}
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
			cont = 370708;
			gtype = 1;
			oklnk = 2;
			area = 370701;
			goal = 370704;
			sort = 370705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 370702;
		}
	}
}

