CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3716;
	title = 371602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 371607;
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
				npcidx = "1163102;";
			}
			CDboTSCheckClrQst
			{
				and = "3715;";
				flink = 0;
				flinknextqid = "3717;";
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
				conv = 371609;
				ctype = 1;
				idx = 1163102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 371608;
				gtype = 1;
				area = 371601;
				goal = 371604;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 371605;
				stype = 2;
				taid = 1;
				title = 371602;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
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
			cont = 371608;
			gtype = 1;
			oklnk = 2;
			area = 371601;
			goal = 371604;
			sort = 371605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 371602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 371614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 371601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1361101;";
			}
		}
	}
}

