CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 944;
	title = 94402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActObjConv
			{
				conv = 94409;
				idx = 65;
				widx = 1;
				ctype = 1;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 94408;
				gtype = 1;
				area = 94401;
				goal = 94404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 94405;
				stype = 1;
				taid = 1;
				title = 94402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 94407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 33;
			}
			CDboTSClickObject
			{
				objidx = "65;";
				widx = 1;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "936;";
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
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 94408;
			gtype = 1;
			oklnk = 2;
			area = 94401;
			goal = 94404;
			sort = 94405;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 94402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 94414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 94401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3144105;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
	}
}

