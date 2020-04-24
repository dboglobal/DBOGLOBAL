CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	rq = 0;
	tid = 6013;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 255;
			prelnk = "3;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "0;";
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckProgQuest
			{
				qid = 6157;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 1;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;";

			CDboTSActTLQ
			{
				idx = 106;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSColObject
			{
				objidx = "645;";
				widx = 1;
			}
		}
	}
}

